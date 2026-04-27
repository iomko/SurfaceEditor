#include "ObjectManipulationLayer.h"
#include "ObjectsLayer.h"
#include "../src/ViewPortsController.h"
#include "../src/Commands/CommandRegistry.h"
#include "../src/Commands/CommandIDs.h"
#include "../src/Tools/ToolRegistry.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/WindowLayerBus.h"
#include "../src/UI/Styling/Window.h"
#include "../src/UI/VisibilityHandler.h"
#include "../src/Builders/UIWindowBuilder.h"

namespace
{
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
}

static AutoRegisterLayerArgs<ObjectManipulationLayer, std::string> reg("OBJECT_MANIPULATION_LAYER");

ObjectManipulationLayer::ObjectManipulationLayer(const std::string& name)
    : Layer(name),
      OverlappingWindow(initWindowConfig())
{
    ViewPortsHolderContext::s_uiLayerController->registerUiWindow(this);
    VisibilityHandler::show(m_windowConfig.name);

    initButtons();
}

ui::styling::WindowConfig ObjectManipulationLayer::initWindowConfig()
{
    auto posConfig  = WindowPosConfigBuilder().posX(0.025f).posY(0.37f).build();
    auto sizeConfig = WindowSizeConfigBuilder().minWidth(0.6f).minHeight(0.6f).build();
    auto flags      = ImGuiWindowFlags_NoTitleBar
                    | ImGuiWindowFlags_NoResize
                    | ImGuiWindowFlags_NoMove
                    | ImGuiWindowFlags_NoScrollbar
                    | ImGuiWindowFlags_NoCollapse;

    return WindowConfigBuilder()
        .name("OBJECT_MANIPULATION_LAYER")
        .background(ui::styling::Color::gray)
        .rounding(12.0f)
        .pos(std::move(posConfig))
        .size(std::move(sizeConfig))
        .flags(flags)
        .build();
}

void ObjectManipulationLayer::initButtons()
{
    static constexpr ui::styling::ButtonConfig defaultConfig{
        17.0f,
        ui::styling::Color::black,
        ui::styling::Color::hoverOverOrange,
        ui::styling::Color::onClickOrange
    };

    m_buttons.emplace_back(std::make_unique<ui::components::RadioImageButton>(cursor, defaultConfig, cursorPath, []() {
        //TODO
    }));
    m_buttons.emplace_back(std::make_unique<ui::components::RadioImageButton>(translate, defaultConfig, translatePath, []() {
        //TODO
    }));
    m_buttons.emplace_back(std::make_unique<ui::components::RadioImageButton>(rotate, defaultConfig, rotatePath, []() {
        //TODO
    }));
    m_buttons.emplace_back(std::make_unique<ui::components::RadioImageButton>(scale, defaultConfig, scalePath, []() {
        //TODO
    }));
    m_buttons.emplace_back(std::make_unique<ui::components::RadioImageButton>(plus, defaultConfig, plusPath, [this]() {
        auto& layerRegistry = LayerRegistry::instance();
        auto* layer         = layerRegistry.getLayer("OBJECTS_LAYER", "ObjectsLayer");
        auto* objectsLayer  = static_cast<ObjectsLayer*>(layer);

        objectsLayer->updatePosition(m_windowConfig.pos.rawPos.x, m_windowConfig.pos.rawPos.y);

        VisibilityHandler::show("OBJECTS_LAYER");
    }));
}

void ObjectManipulationLayer::resetModeState()
{
    for (auto& button : m_buttons)
    {
        button->setIsSelected(false);
        button->config().background = ui::styling::Color::black;
    }

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

void ObjectManipulationLayer::onImGuiRender()
{
    ui::styling::Window::setPosAndSize(m_windowConfig.name, m_windowConfig.pos, m_windowConfig.size);
    if (!VisibilityHandler::isVisible(m_windowConfig.name))
    {
        return;
    }

    ui::styling::Window::init(m_windowConfig);

    ui::styling::Window::addToLayout([this]() {
        const float iconSize = ImGui::GetMainViewport()->WorkSize.x * 0.015f;

        for (auto& button : m_buttons)
        {
            ui::styling::Button::init(button->config());

            if (ImGui::ImageButton(button->name().c_str(), button->textureID(), ImVec2(iconSize, iconSize)))
            {
                resetModeState();

                button->setIsSelected(true);
                button->config().background = ui::styling::Color::selectedOrange;
                button->execute();
            }

            ui::styling::Button::destroy(button->config());
        }
    });
    
    ui::styling::Window::destroy(m_windowConfig);
}
