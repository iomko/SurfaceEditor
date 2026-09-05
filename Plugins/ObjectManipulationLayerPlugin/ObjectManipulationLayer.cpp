#include "ObjectManipulationLayer.h"
#include "../src/ViewPortsController.h"
#include "../src/Commands/CommandRegistry.h"
#include "../src/Commands/CommandIDs.h"
#include "../src/Tools/ToolRegistry.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/WindowLayerBus.h"
#include "../src/UI/Styling/Window.h"
#include "../src/UI/Styling/Button.h"
#include "../src/UI/Components/RadioImageButton.h"
#include "../src/UI/VisibilityHandler.h"
#include "../src/UI/Events.h"
#include "../src/Builders/UIWindowBuilder.h"
#include "../src/Builders/UIButtonBuilder.h"

namespace
{
    static constexpr const char* LAYER_NAME = "OBJECT_MANIPULATION_LAYER";

    //Names
    static constexpr const char* cursor    = "cursor";
    static constexpr const char* translate = "translate";
    static constexpr const char* rotate    = "rotate";
    static constexpr const char* scale     = "scale";
    static constexpr const char* plus      = "plus";

    //Paths
    static constexpr const char* cursorPath    = "../images/gui/cursor.png";
    static constexpr const char* translatePath = "../images/gui/translate.png";
    static constexpr const char* rotatePath    = "../images/gui/rotate.png";
    static constexpr const char* scalePath     = "../images/gui/scale.png";
    static constexpr const char* plusPath      = "../images/gui/plus.png";
} // namespace

static AutoRegisterLayerArgs<ObjectManipulationLayer, std::string> reg(LAYER_NAME);

ObjectManipulationLayer::ObjectManipulationLayer(const std::string& name)
    : Layer(name)
    , ui::IWindow(LAYER_NAME)
{
    VisibilityHandler::show(LAYER_NAME);

    initWindowConfig();
    initComponents();
    initConnections();
}

void ObjectManipulationLayer::initConnections()
{
    WindowLayerBus::on<ObjectsLayerState>([this](ObjectsLayerState& state) {
        auto* addMeshButtonConfig = dynamic_cast<ui::styling::ButtonConfig*>(m_addMeshButton->config());
        ImVec2 objectsLayerPos{
            m_windowConfig.pos.rawPos.x + m_windowConfig.size.realSize.x - addMeshButtonConfig->realSize.x,
            m_windowConfig.pos.rawPos.y + m_windowConfig.size.realSize.y - addMeshButtonConfig->realSize.y
        };
        state.windowPos = objectsLayerPos;
    });
}

void ObjectManipulationLayer::initWindowConfig()
{
    auto posConfig  = WindowPosConfigBuilder().posX(0.025f).posY(0.37f).build();
    auto sizeConfig = WindowSizeConfigBuilder().minWidth(0.6f).minHeight(0.6f).build();
    auto flags      = ImGuiWindowFlags_NoTitleBar
                    | ImGuiWindowFlags_NoResize
                    | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoScrollbar
                    | ImGuiWindowFlags_NoCollapse;

    m_windowConfig = WindowConfigBuilder()
        .name(LAYER_NAME)
        .background(ui::styling::Color::gray)
        .rounding(12.0f)
        .pos(std::move(posConfig))
        .size(std::move(sizeConfig))
        .flags(flags)
        .build();
}

void ObjectManipulationLayer::initComponents()
{
    using namespace ui::components;

    static constexpr int itemsCount = 5;
    m_radioButtons.reserve(itemsCount);

    auto defaultConfig = ButtonConfigBuilder()
        .rounding(17.0f)
        .size(ImVec2{ 0.025f, 0.025f })
        .square(true)
        .framePadding(ImVec2{ 5.0f, 5.0f })
        .background(ui::styling::Color::black)
        .onHoverColor(ui::styling::Color::hoverOverOrange)
        .onClickColor(ui::styling::Color::onClickOrange)
        .build();

    ui::Layout* layout = emplaceLayout();
    layout->reserveComponents(itemsCount);

    m_radioButtons.push_back(emplaceComponent<RadioImageButton>(layout, cursor, cursorPath, m_radioButtons, defaultConfig, [this](Button*) {
        activateSelectionMode();
    }));
    m_radioButtons.push_back(emplaceComponent<RadioImageButton>(layout, translate, translatePath, m_radioButtons, defaultConfig, [this](Button*) {
        activateGizmoMode(ImGuizmo::OPERATION::TRANSLATE);
    }));
    m_radioButtons.push_back(emplaceComponent<RadioImageButton>(layout, rotate, rotatePath, m_radioButtons, defaultConfig, [this](Button*) {
        activateGizmoMode(ImGuizmo::OPERATION::ROTATE);
    }));
    m_radioButtons.push_back(emplaceComponent<RadioImageButton>(layout, scale, scalePath, m_radioButtons, defaultConfig, [this](Button*) {
        activateGizmoMode(ImGuizmo::OPERATION::SCALE);
    }));
    m_addMeshButton = emplaceComponent<RadioImageButton>(layout, plus, plusPath, m_radioButtons, defaultConfig, [this](Button* button) {
        resetModeState();
        
        auto* radioButton = dynamic_cast<ui::components::RadioButton*>(button);
        radioButton->setIsSelected(false);
        
        VisibilityHandler::show("OBJECTS_LAYER");
    });
    m_radioButtons.push_back(m_addMeshButton);
}

void ObjectManipulationLayer::resetModeState()
{
    VisibilityHandler::hide("GIZMO_LAYER");
    VisibilityHandler::hide("OBJECTS_LAYER");

    ViewPortsHolderContext::s_selectionController->setSelectionModeActive(false);

    if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
    {
        delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
        ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
    }
    ViewPortsHolderContext::s_viewPortsController->m_currentTool = nullptr;
}

void ObjectManipulationLayer::activateSelectionMode()
{
    resetModeState();

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
}

void ObjectManipulationLayer::activateGizmoMode(ImGuizmo::OPERATION operationType)
{
    resetModeState();

    GizmoLayerState state;
    state.operationType = operationType;

    WindowLayerBus::emit(state);
    VisibilityHandler::show("GIZMO_LAYER");
}

void ObjectManipulationLayer::onImGuiRender()
{
    this->render();
}
