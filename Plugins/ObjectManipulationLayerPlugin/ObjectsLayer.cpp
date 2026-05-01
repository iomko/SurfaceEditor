#include "ObjectsLayer.h"
#include "../src/UI/Styling/Font.h"
#include "../src/Commands/CommandRegistry.h"
#include "../src/Commands/CommandIDs.h"
#include "../src/Builders/UIWindowBuilder.h"
#include "../src/Builders/UIButtonBuilder.h"

static AutoRegisterLayerArgs<ObjectsLayer, std::string> reg("OBJECTS_LAYER");

ObjectsLayer::ObjectsLayer(const std::string& name)
    : Layer(name),
      OverlappingWindow(initWindowConfig())
{
    initButtons();
}

void ObjectsLayer::initButtons()
{
    using namespace ui::components;

    auto buttonSize = ImVec2{m_windowConfig.size.width * 0.8f, m_windowConfig.size.height * 0.25f};

    auto defaultConfig = ButtonConfigBuilder()
        .rounding(12.0f)
        .size(buttonSize)
        .background(ui::styling::Color::darkGray)
        .font(ui::styling::Font::regular(0.8f))
        .size(ImVec2{ 0.04f, 0.02f })
        .onHoverColor(ui::styling::Color::hoverOverOrange)
        .onClickColor(ui::styling::Color::onClickOrange)
        .build();

    m_buttons.emplace_back(std::make_unique<Button>("Plane", defaultConfig, [](Button*) {
        // AdditionLayer::setAdditionType(AdditionType::PLANE);
    }));
    m_buttons.emplace_back(std::make_unique<Button>("Cube", defaultConfig, [](Button*) {
        // AdditionLayer::setAdditionType(AdditionType::CUBE);
    }));
    m_buttons.emplace_back(std::make_unique<Button>("Surface", defaultConfig, [](Button*) {
        // AdditionLayer::setAdditionType(AdditionType::SURFACE);
    }));
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

    m_windowConfig.pos = WindowPosConfigBuilder().relativePosX(pos.x).relativePosY(pos.y).build();
}

ui::styling::WindowConfig ObjectsLayer::initWindowConfig()
{
    auto sizeConfig = WindowSizeConfigBuilder().width(0.05f).height(0.09f).minHeight(0.6f).minWidth(0.6f).build();
    auto flags      = ImGuiWindowFlags_NoTitleBar
                    | ImGuiWindowFlags_NoResize
                    | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoScrollbar
                    | ImGuiWindowFlags_NoCollapse;

    return WindowConfigBuilder()
        .name("OBJECTS_LAYER")
        .background(ui::styling::Color::gray)
        .rounding(17.0f)
        .size(sizeConfig)
        .flags(flags)
        .build();
}

void ObjectsLayer::onImGuiRender()
{
    if (m_getPos)
    {
        updatePosition();
    }
    
    ui::styling::Window::setRelativePosAndSize(m_windowConfig);

    if (!VisibilityHandler::isVisible(m_windowConfig.name))
    {
        return;
    }

    ui::styling::Window::init(m_windowConfig);

    for (auto& button : m_buttons)
    {
        ui::styling::Button::init(button->config());

        const ImVec2 buttonSize = button->config().realSize;

        if (ImGui::Button(button->name().c_str(), buttonSize))
        {
            button->execute();

            VisibilityHandler::hide(m_windowConfig.name);
            // VisibilityHandler::show("ADDITION_LAYER");

            m_onFinish();
        }

        ui::styling::Button::destroy(button->config());
    }

    ui::styling::Window::destroy(m_windowConfig);
}