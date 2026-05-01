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
#include "../src/Builders/UIButtonBuilder.h"

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
    using namespace ui::components;

    auto defaultConfig = ButtonConfigBuilder()
        .rounding(17.0f)
        .size(ImVec2{ 0.025f, 0.025f })
        .square(true)
        .framePadding(ImVec2{ 5.0f, 5.0f })
        .background(ui::styling::Color::black)
        .onHoverColor(ui::styling::Color::hoverOverOrange)
        .onClickColor(ui::styling::Color::onClickOrange)
        .build();

    m_buttons.emplace_back(std::make_unique<RadioImageButton>(cursor, defaultConfig, cursorPath, [](Button*) {
        //TODO
    }));
    m_buttons.emplace_back(std::make_unique<RadioImageButton>(translate, defaultConfig, translatePath, [](Button*) {
        //TODO
    }));
    m_buttons.emplace_back(std::make_unique<RadioImageButton>(rotate, defaultConfig, rotatePath, [](Button*) {
        //TODO
    }));
    m_buttons.emplace_back(std::make_unique<RadioImageButton>(scale, defaultConfig, scalePath, [](Button*) {
        //TODO
    }));
    m_buttons.emplace_back(std::make_unique<RadioImageButton>(plus, defaultConfig, plusPath, [this](Button* button) {
        auto& layerRegistry = LayerRegistry::instance();
        auto* layer         = layerRegistry.getLayer("OBJECTS_LAYER", "ObjectsLayer");
        auto* objectsLayer  = static_cast<ObjectsLayer*>(layer);

        objectsLayer->setOnFinishCallback([this]() {
            resetModeState();
        });
        objectsLayer->setPosCallback([this, button]() {
            return ImVec2{
                m_windowConfig.pos.rawPos.x + m_windowConfig.size.realSize.x - button->config().realSize.x,
                m_windowConfig.pos.rawPos.y + m_windowConfig.size.realSize.y - button->config().realSize.y
            };
        });

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
    ui::styling::Window::setPosAndSize(m_windowConfig);
    if (!VisibilityHandler::isVisible(m_windowConfig.name))
    {
        return;
    }

    ui::styling::Window::init(m_windowConfig);

    ui::styling::Window::addToLayout([this]() {
        for (auto& button : m_buttons)
        {
            ui::styling::Button::init(button->config());

            const ImVec2 buttonSize = button->config().realSize;

            if (ImGui::ImageButton(button->name().c_str(), button->textureID(), buttonSize))
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
