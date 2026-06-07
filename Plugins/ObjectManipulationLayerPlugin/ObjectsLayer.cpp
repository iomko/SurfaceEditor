#include "ObjectsLayer.h"
#include "../src/UI/Styling/Font.h"
#include "../src/Commands/CommandRegistry.h"
#include "../src/Commands/CommandIDs.h"
#include "../src/Builders/UIWindowBuilder.h"
#include "../src/Builders/UIButtonBuilder.h"
#include "../AdditionLayerPlugin/AdditionLayer.h"

namespace
{
    static constexpr const char* LAYER_NAME = "OBJECTS_LAYER";
} // namespace

static AutoRegisterLayerArgs<ObjectsLayer, std::string> reg("OBJECTS_LAYER");

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

    auto& layerRegistry = LayerRegistry::instance();
    auto* layer         = layerRegistry.getLayer("ADDITION_LAYER", "AdditionLayer");
    auto* additionLayer = static_cast<AdditionLayer*>(layer);

    ui::Layout* layout = emplaceLayout();
    layout->reserveComponents(itemsCount);

    emplaceComponent<Button>(layout, "Plane", defaultConfig, [this, additionLayer](Button*) {
        additionLayer->setAdditionType(AdditionType::PLANE);
        invokeAdditionLayer();
    });
    emplaceComponent<Button>(layout, "Cube", defaultConfig, [this, additionLayer](Button*) {
        additionLayer->setAdditionType(AdditionType::CUBE);
        invokeAdditionLayer();
    });
    emplaceComponent<Button>(layout, "Surface", defaultConfig, [this, additionLayer](Button*) {
        additionLayer->setAdditionType(AdditionType::SURFACE);
        invokeAdditionLayer();
    });
}

void ObjectsLayer::setOnFinishCallback(std::function<void()> onFinishCallback)
{
    m_onFinish = std::move(onFinishCallback);
}

void ObjectsLayer::setPosCallback(std::function<ImVec2()> getPosCallback)
{
    m_getPos = std::move(getPosCallback);
}

void ObjectsLayer::updatePosition()
{
    ImVec2 pos = m_getPos();

    m_windowConfig.pos = WindowPosConfigBuilder().relativePosition(true).relativePosX(pos.x).relativePosY(pos.y).build();
}

void ObjectsLayer::invokeAdditionLayer()
{
    VisibilityHandler::hide(LAYER_NAME);
    VisibilityHandler::show("ADDITION_LAYER");

    m_onFinish();
}

void ObjectsLayer::onImGuiRender()
{
    if (m_getPos)
    {
        updatePosition();
    }
    
    this->render();
}