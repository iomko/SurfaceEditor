#include "ObjectManipulationLayer.h"
#include "../src/ViewPortsController.h"
#include "../src/Commands/CommandRegistry.h"
#include "../src/Commands/CommandIDs.h"
#include "../src/Tools/ToolRegistry.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/WindowLayerBus.h"
#include "../src/UI/Events.h"
#include <Builders/UIWindowBuilder.h>
#include <Builders/UIButtonBuilder.h>
#include <Styling/Window.h>
#include <Styling/Button.h>
#include <Components/RadioImageButton.h>
#include <Placements/ViewportPlacement.h>
#include <Utils/VisibilityHandler.h>

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
    , ui::components::Window(LAYER_NAME)
{
    VisibilityHandler::instance().show(LAYER_NAME);
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
        .size(std::move(sizeConfig))
        .flags(flags)
        .build();
}

std::unique_ptr<ui::placements::Placement> ObjectManipulationLayer::createPlacement()
{
    return std::make_unique<ui::placements::ViewportPlacement>(0.025f, 0.37f);
}

void ObjectManipulationLayer::initComponents()
{
    using namespace ui::components;

    static constexpr int itemsCount = 5;

    auto defaultConfig = ButtonConfigBuilder()
        .rounding(17.0f)
        .size(ImVec2{ 0.025f, 0.025f })
        .square(true)
        .framePadding(ImVec2{ 5.0f, 5.0f })
        .background(ui::styling::Color::black)
        .onHoverColor(ui::styling::Color::hoverOverOrange)
        .onClickColor(ui::styling::Color::onClickOrange)
        .build();

    ui::components::Layout* layout = emplaceLayout();
    layout->reserveComponents(itemsCount);

    emplaceComponent<RadioImageButton>(layout, cursor, cursorPath, m_radioGroup, defaultConfig, [this](Button*) {
        activateSelectionMode();
    });
    emplaceComponent<RadioImageButton>(layout, translate, translatePath, m_radioGroup, defaultConfig, [this](Button*) {
        activateGizmoMode(ImGuizmo::OPERATION::TRANSLATE);
    });
    emplaceComponent<RadioImageButton>(layout, rotate, rotatePath, m_radioGroup, defaultConfig, [this](Button*) {
        activateGizmoMode(ImGuizmo::OPERATION::ROTATE);
    });
    emplaceComponent<RadioImageButton>(layout, scale, scalePath, m_radioGroup, defaultConfig, [this](Button*) {
        activateGizmoMode(ImGuizmo::OPERATION::SCALE);
    });
    m_addMeshButton = emplaceComponent<RadioImageButton>(layout, plus, plusPath, m_radioGroup, defaultConfig, [this](Button* button) {
        resetModeState();

        auto* radioButton = dynamic_cast<ui::components::RadioButton*>(button);
        radioButton->setIsSelected(false);

        VisibilityHandler::instance().show("OBJECTS_LAYER");
    });
}

void ObjectManipulationLayer::resetModeState()
{
    VisibilityHandler::instance().hide("GIZMO_LAYER");
    VisibilityHandler::instance().hide("OBJECTS_LAYER");

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
    VisibilityHandler::instance().show("GIZMO_LAYER");
}

void ObjectManipulationLayer::onImGuiRender()
{
    this->render();
}
