#include "ObjectManipulationLayer.h"
#include "ObjectsLayer.h"
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
        //TODO
        resetModeState();
    }));
    m_radioButtons.push_back(emplaceComponent<RadioImageButton>(layout, translate, translatePath, m_radioButtons, defaultConfig, [this](Button*) {
        //TODO
        resetModeState();
    }));
    m_radioButtons.push_back(emplaceComponent<RadioImageButton>(layout, rotate, rotatePath, m_radioButtons, defaultConfig, [this](Button*) {
        //TODO
        resetModeState();
    }));
    m_radioButtons.push_back(emplaceComponent<RadioImageButton>(layout, scale, scalePath, m_radioButtons, defaultConfig, [this](Button*) {
        //TODO
        resetModeState();
    }));
    m_radioButtons.push_back(emplaceComponent<RadioImageButton>(layout, plus, plusPath, m_radioButtons, defaultConfig, [this](Button* button) {
        resetModeState();
        invokeObjectsLayer(button);
    }));
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

void ObjectManipulationLayer::invokeObjectsLayer(ui::components::Button* button)
{
    auto* config = dynamic_cast<ui::styling::ButtonConfig*>(button->config());

    auto& layerRegistry = LayerRegistry::instance();
    auto* layer         = layerRegistry.getLayer("OBJECTS_LAYER", "ObjectsLayer");
    auto* objectsLayer  = static_cast<ObjectsLayer *>(layer);

    objectsLayer->setOnFinishCallback([this]() {
        resetModeState();
    });
    objectsLayer->setPosCallback([this, config]() {
        return ImVec2{
            m_windowConfig.pos.rawPos.x + m_windowConfig.size.realSize.x - config->realSize.x,
            m_windowConfig.pos.rawPos.y + m_windowConfig.size.realSize.y - config->realSize.y};
    });

    VisibilityHandler::show("OBJECTS_LAYER");
}

void ObjectManipulationLayer::onImGuiRender()
{
    this->render();
}
