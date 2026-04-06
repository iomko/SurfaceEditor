#include "ObjectManipulationLayer.h"
#include "../src/ViewPortsController.h"
#include "../src/Commands/CommandRegistry.h"
#include "../src/Commands/CommandIDs.h"
#include "../src/Tools/ToolRegistry.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/WindowLayerBus.h"

namespace
{
    //Names
    static constexpr const char* cursor = "cursor";
    static constexpr const char* translate = "translate";
    static constexpr const char* rotate = "rotate";
    static constexpr const char* scale = "scale";
    static constexpr const char* plus = "plus";

    //Paths
    static constexpr const char* cursorPath = "../images/gui/cursor.png";
    static constexpr const char* translatePath = "../images/gui/translate.png";
    static constexpr const char* rotatePath = "../images/gui/rotate.png";
    static constexpr const char* scalePath = "../images/gui/scale.png";
    static constexpr const char* plusPath = "../images/gui/plus.png";
}

static AutoRegisterLayerArgs<ObjectManipulationLayer, std::string> reg("OBJECT_MANIPULATION_LAYER");

ObjectManipulationLayer::ObjectManipulationLayer(const std::string& name)
    : Layer(name), m_imagesLoaded{}, m_iconSize{}, m_rightBottomCorner{}
{
    ViewPortsHolderContext::s_uiLayerController->registerUiWindow(this);
}

void ObjectManipulationLayer::loadPanelImages()
{
    VisibilityHandler::show("OBJECT_MANIPULATION_LAYER");

    // Selection
    m_buttons.emplace_back(std::make_unique<ImageButton>(cursor, cursorPath, [](Button*) {
        ViewPortsHolderContext::s_selectionController->setSelectionModeActive(true);
        auto selectionMode = ViewPortsHolderContext::s_selectionController->selectionMode();
        ITool* tool{};

        switch (selectionMode)
        {
            case SelectionMode::MESH:
                tool = ToolRegistry::instance().getTool("MESH_SELECTION_TOOL");
                break;
            case SelectionMode::FACE:
                // tool = ToolRegistry::instance().getTool("FACE_SELECTION_TOOL");
                break;
            case SelectionMode::EDGE:
                // TODO
                break;
            case SelectionMode::VERTEX:
                // TODO
                break;
            default:
                break;
        }

        if (tool == nullptr)
        {
            return;
        }
        ViewPortsHolderContext::s_viewPortsController->m_currentTool = tool;
        if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
        {
            delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
            ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
        }

    }, []() {
        ViewPortsHolderContext::s_selectionController->setSelectionModeActive(false);

        if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
        {
            delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
            ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
        }
        ViewPortsHolderContext::s_viewPortsController->m_currentTool = nullptr;
    }));

    // Move gizmo
    m_buttons.emplace_back(std::make_unique<ImageButton>(translate, translatePath, [](Button*) {        
        GizmoParams params;
        params.m_type = ImGuizmo::OPERATION::TRANSLATE;

        GizmoLayer::init(params);
        VisibilityHandler::show("GIZMO_LAYER");
    }, []() {
        VisibilityHandler::hide("GIZMO_LAYER");
    }));

    // Rotate gizmo
    m_buttons.emplace_back(std::make_unique<ImageButton>(rotate, rotatePath, [](Button*) {
        GizmoParams params;
        params.m_type = ImGuizmo::OPERATION::ROTATE;
        
        GizmoLayer::init(params);
        VisibilityHandler::show("GIZMO_LAYER");
    }, []() {
        VisibilityHandler::hide("GIZMO_LAYER");
    }));

    // Scale gizmo
    m_buttons.emplace_back(std::make_unique<ImageButton>(scale, scalePath, [](Button*) {
        GizmoParams params;
        params.m_type = ImGuizmo::OPERATION::SCALE;
        
        GizmoLayer::init(params);
        VisibilityHandler::show("GIZMO_LAYER");
    }, []() {
        VisibilityHandler::hide("GIZMO_LAYER");
    }));

    // Add object
    m_buttons.emplace_back(std::make_unique<ImageButton>(plus, plusPath, [](Button* button) {
        auto imageButton = dynamic_cast<ImageButton*>(button);
        
        if (imageButton)
        {
            imageButton->isSelected() = false;
        }
        
        VisibilityHandler::show("OBJECTS");
    }, [this]() {
        VisibilityHandler::hide("OBJECTS");
        VisibilityHandler::hide("ADDITION_LAYER");
    }));

    m_imagesLoaded = true;
}

void ObjectManipulationLayer::setWindowSizeAndPosition()
{
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    float viewportWidth  = viewport->WorkSize.x;
    float viewportHeight = viewport->WorkSize.y;

    m_iconSize = viewportWidth * 0.015f;

    float framePadding  = ImGui::GetStyle().FramePadding.x;
    float windowPadding = ImGui::GetStyle().WindowPadding.x;

    float windowWidth  = m_iconSize + 2.0f * framePadding + 2.0f * windowPadding;
    float windowHeight = m_buttons.size() * (m_iconSize + 2.0f * framePadding) + 2.0f * windowPadding;

    float leftInset = viewportWidth * 0.02f;

    float posX = viewport->WorkPos.x + leftInset;
    float posY = viewport->WorkPos.y + (viewportHeight - windowHeight) * 0.5f;

    m_pos  = ImVec2(posX, posY);
    m_size = ImVec2(windowWidth, windowHeight);

    m_rightBottomCorner = { posX + windowWidth - (m_iconSize / 2), posY + windowHeight - (m_iconSize / 2) };

    ImGui::SetNextWindowSize(m_size, ImGuiCond_Always);
    ImGui::SetNextWindowPos(m_pos, ImGuiCond_Always);

    WindowStyle::checkResolutionRange("OBJECT_MANIPULATION_LAYER", viewportHeight, viewportWidth);
}

void ObjectManipulationLayer::onImGuiRender()
{
    if (!m_imagesLoaded)
    {
        loadPanelImages();
    }

    setWindowSizeAndPosition();

    if (!VisibilityHandler::isVisible("OBJECT_MANIPULATION_LAYER"))
    {
        return;
    }
    
    int windowColorStylesApplied{}, windowVarStylesApplied{};

    static const ImGuiWindowFlags flags = WindowStyle::defaultWindow();
    WindowStyle::setup(this->getName().c_str(), flags, windowColorStylesApplied, windowVarStylesApplied);

    for (auto& button : m_buttons)
    {
        int apppliedColorStyles{}, appliedVarStyles{};

        ButtonStyle::setRadioButtonBackground(button.get(), apppliedColorStyles);
        ButtonStyle::controlPanelStyle(apppliedColorStyles, appliedVarStyles);

        if (ImGui::ImageButton(button->name().c_str(), button->textureID(), ImVec2(m_iconSize, m_iconSize)))
        {
            for (auto& unselectedButton : m_buttons)
            {
                if (button != unselectedButton)
                {
                    unselectedButton->isSelected() = false;
                    unselectedButton->end();
                }
            }

            button->isSelected() = true;
            button->execute();
        }
        
        ButtonStyle::closeStyling(apppliedColorStyles, appliedVarStyles);
    }

    WindowStyle::end(windowColorStylesApplied, windowVarStylesApplied);
}
