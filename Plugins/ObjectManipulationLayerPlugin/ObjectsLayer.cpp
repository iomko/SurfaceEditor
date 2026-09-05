#include "ObjectsLayer.h"
#include "../src/UI/Styling/Font.h"
#include "../src/UI/WindowLayerBus.h"
#include "../src/UI/Events.h"
#include "../src/Commands/CommandRegistry.h"
#include "../src/Commands/CommandIDs.h"
#include "../src/Builders/UIWindowBuilder.h"
#include "../src/Builders/UIButtonBuilder.h"

namespace
{
    static constexpr const char* LAYER_NAME = "OBJECTS_LAYER";
} // namespace

static AutoRegisterLayerArgs<ObjectsLayer, std::string> reg(LAYER_NAME);

ObjectsLayer::ObjectsLayer(const std::string& name)
    : Layer(name),
      ui::IWindow(LAYER_NAME)
{
    initWindowConfig();
    initComponents();
}

void ObjectsLayer::initWindowConfig()
{
    auto sizeConfig = WindowSizeConfigBuilder().width(0.05f).height(0.09f).minHeight(0.6f).minWidth(0.6f).build();
    auto flags      = ImGuiWindowFlags_NoTitleBar
                    | ImGuiWindowFlags_NoResize
                    | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoScrollbar
                    | ImGuiWindowFlags_NoCollapse;

    m_windowConfig = WindowConfigBuilder()
        .name(LAYER_NAME)
        .background(ui::styling::Color::gray)
        .rounding(17.0f)
        .size(sizeConfig)
        .flags(flags)
        .build();
}

void ObjectsLayer::initComponents()
{
    using namespace ui::components;

    static constexpr int itemsCount = 3;

    auto defaultConfig = ButtonConfigBuilder()
        .rounding(12.0f)
        .background(ui::styling::Color::darkGray)
        .font(ui::styling::Font::regular(0.8f))
        .size(ImVec2{ 0.04f, 0.02f })
        .onHoverColor(ui::styling::Color::hoverOverOrange)
        .onClickColor(ui::styling::Color::onClickOrange)
        .build();

    ui::Layout* layout = emplaceLayout();
    layout->reserveComponents(itemsCount);

    emplaceComponent<Button>(layout, "Plane", defaultConfig, [this](Button*) {
        invokeAdditionLayer(AdditionType::PLANE);
    });
    emplaceComponent<Button>(layout, "Cube", defaultConfig, [this](Button*) {
        invokeAdditionLayer(AdditionType::CUBE);
    });
    emplaceComponent<Button>(layout, "Surface", defaultConfig, [this](Button*) {
        invokeAdditionLayer(AdditionType::SURFACE);
    });
}

void ObjectsLayer::invokeAdditionLayer(AdditionType additionType)
{
    AdditionLayerState additionLayerState;
    additionLayerState.additionType = additionType;
    
    WindowLayerBus::emit(additionLayerState);
    VisibilityHandler::hide(LAYER_NAME);
    VisibilityHandler::show("ADDITION_LAYER");
}

void ObjectsLayer::updatePos(ImVec2& actualPosition)
{
    m_windowConfig.pos = WindowPosConfigBuilder()
        .relativePosition(true)
        .relativePosX(actualPosition.x)
        .relativePosY(actualPosition.y)
        .build();
}

void ObjectsLayer::onImGuiRender()
{
    ObjectsLayerState currState;
    WindowLayerBus::emit(currState);
    updatePos(currState.windowPos);

    this->render();
}