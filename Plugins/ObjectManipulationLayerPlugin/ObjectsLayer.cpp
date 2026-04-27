#include "ObjectsLayer.h"
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
    auto defaultConfig = ButtonConfigBuilder()
        .rounding(12.0f)
        .background(ui::styling::Color::black)
        .onHoverColor(ui::styling::Color::hoverOverOrange)
        .onClickColor(ui::styling::Color::onClickOrange)
        // .framePadding()
        // .windowPadding()
        .build();

    m_buttons.emplace_back(std::make_unique<ui::components::Button>("Plane", defaultConfig, []() {
        // AdditionLayer::setAdditionType(AdditionType::PLANE);
    }));
    m_buttons.emplace_back(std::make_unique<ui::components::Button>("Cube", defaultConfig, []() {
        // AdditionLayer::setAdditionType(AdditionType::CUBE);
    }));
    m_buttons.emplace_back(std::make_unique<ui::components::Button>("Surface", defaultConfig, []() {
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

    m_windowConfig.pos = WindowPosConfigBuilder().relativePosX(pos.x * 0.9f).relativePosY(pos.y * 0.95f).build();
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
        ImVec2 windowSize = ImGui::GetWindowSize();
        ImVec2 buttonSize{
            windowSize.x * 0.7f,
            windowSize.y * 0.25f
        };

        ui::styling::Button::init(button->config());

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