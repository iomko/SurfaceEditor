#include "ObjectManipulationLayer.h"
#include "../src/ViewPortsController.h"
#include "../src/Commands/CommandRegistry.h"
#include "../src/Commands/CommandIDs.h"
#include "../src/Tools/ToolRegistry.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/WindowLayerBus.h"
#include "../src/UI/Styling/Window.h"
#include "../src/UI/VisibilityHandler.h"

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
    : Layer(name)
{
    VisibilityHandler::show("OBJECT_MANIPULATION_LAYER");

    setupWindow();
    initButtons();
}

void ObjectManipulationLayer::initWindowPosConfig()
{
    auto& posConfig = m_windowConfig.pos;
    posConfig.posX  = 0.025f;
    posConfig.posY  = 0.37f;
}

void ObjectManipulationLayer::initWindowSizeConfig()
{
    auto& sizeConfig     = m_windowConfig.size;
    sizeConfig.minWidth  = 0.6f;
    sizeConfig.minHeight = 0.6f;
}

void ObjectManipulationLayer::initWindowConfig()
{
    m_windowConfig.name                      = this->getName().c_str();
    m_windowConfig.layerName                 = "OBJECT_MANIPULATION_LAYER";
    m_windowConfig.backgroundColor           = ui::styling::Color::gray;
    m_windowConfig.rounding                  = 12.0f;
    m_windowConfig.flags = ImGuiWindowFlags_NoTitleBar
                         | ImGuiWindowFlags_NoResize
                         | ImGuiWindowFlags_NoMove
                         | ImGuiWindowFlags_NoScrollbar
                         | ImGuiWindowFlags_NoCollapse;;
}

void ObjectManipulationLayer::initButtons()
{
    m_buttonConfig.rounding         = 17.0f;
    m_buttonConfig.background       = ui::styling::Color::black;
    m_buttonConfig.onHoverOverColor = ui::styling::Color::hoverOverOrange;
    m_buttonConfig.onClickColor     = ui::styling::Color::onClickOrange;

    m_buttons.emplace_back(std::make_unique<ui::components::RadioImageButton>(cursor, cursorPath, []() {
        //TODO
    }));
    m_buttons.emplace_back(std::make_unique<ui::components::RadioImageButton>(translate, translatePath, []() {
        //TODO
    }));
    m_buttons.emplace_back(std::make_unique<ui::components::RadioImageButton>(rotate, rotatePath), []() {
        //TODO
    });
    m_buttons.emplace_back(std::make_unique<ui::components::RadioImageButton>(scale, scalePath), []() {
        //TODO
    });
    m_buttons.emplace_back(std::make_unique<ui::components::RadioImageButton>(plus, plusPath), []() {
        //TODO
    });
}

void ObjectManipulationLayer::resetModeState()
{
    for (auto& button : m_buttons)
    {
        button->setIsSelected(false);
        
        // ui::styling::Button::changeBackgroundColor(button)
    }

    VisibilityHandler::hide("GIZMO_LAYER");

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
    ui::styling::Window::setPosAndSize(m_windowConfig.layerName, m_windowConfig.pos, m_windowConfig.size);
    if (!VisibilityHandler::isVisible("OBJECT_MANIPULATION_LAYER"))
    {
        return;
    }

    ui::styling::Window::init(m_windowConfig);

    ui::styling::Window::addToLayout([this]() {
        const float iconSize = ImGui::GetMainViewport()->WorkSize.x * 0.015f;

        ui::styling::Button::init(m_buttonConfig);

        for (auto& button : m_buttons)
        {
            if (ImGui::ImageButton(button->name().c_str(), button->textureID(), ImVec2(iconSize, iconSize)))
            {
                button->setIsSelected(true);

                resetModeState();

                //change background color

                button->execute();
            }
        }

        ui::styling::Button::destroy(m_buttonConfig);
    });
    
    ui::styling::Window::destroy(m_windowConfig);
}
