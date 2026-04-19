#include "ObjectManipulationLayer.h"
#include "../src/ViewPortsController.h"
#include "../src/Commands/CommandRegistry.h"
#include "../src/Commands/CommandIDs.h"
#include "../src/Tools/ToolRegistry.h"
#include "../src/UI/LayerRegistry.h"
#include "../src/UI/WindowLayerBus.h"
#include "../src/UI/Components/Window.h"
#include "../src/UI/VisibilityHandler.h"

namespace
{
    //Names
    static constexpr const char* cursor = "cursor";
    static constexpr const char* translate = "translate";
    static constexpr const char* rotate = "rotate";
    static constexpr const char* scale = "scale";
    static constexpr const char* plus = "plus";

    //Paths
    static constexpr const char* cursorPath = "../images/gui/cursor.png";
    static constexpr const char* translatePath = "../images/gui/translate.png";
    static constexpr const char* rotatePath = "../images/gui/rotate.png";
    static constexpr const char* scalePath = "../images/gui/scale.png";
    static constexpr const char* plusPath = "../images/gui/plus.png";
}

static AutoRegisterLayerArgs<ObjectManipulationLayer, std::string> reg("OBJECT_MANIPULATION_LAYER");

ObjectManipulationLayer::ObjectManipulationLayer(const std::string& name)
    : Layer(name)
{
    ViewPortsHolderContext::s_uiLayerController->registerUiWindow(this);
    VisibilityHandler::show("OBJECT_MANIPULATION_LAYER");

    initButtons();
    initWindowPosConfig();
    initWindowConfig();
}

void ObjectManipulationLayer::initButtons()
{
    m_buttons.emplace_back(std::make_unique<ImageButton>(cursor, cursorPath));
    m_buttons.emplace_back(std::make_unique<ImageButton>(translate, translatePath));
    m_buttons.emplace_back(std::make_unique<ImageButton>(rotate, rotatePath));
    m_buttons.emplace_back(std::make_unique<ImageButton>(scale, scalePath));
    m_buttons.emplace_back(std::make_unique<ImageButton>(plus, plusPath));
}

void ObjectManipulationLayer::initWindowPosConfig()
{
    m_windowPosConfig.layer     = "OBJECT_MANIPULATION_LAYER";
    m_windowPosConfig.posX      = 0.025f;
    m_windowPosConfig.posY      = 0.37f;
    m_windowPosConfig.width     = 0.0f; // implicit
    m_windowPosConfig.height    = 0.0f; // implicit
    m_windowPosConfig.minWidth  = 0.6f;
    m_windowPosConfig.minHeight = 0.6f;
}

void ObjectManipulationLayer::initWindowConfig()
{
    m_windowConfig.name                      = this->getName().c_str();
    m_windowConfig.transparent               = false;
    m_windowConfig.rounding                  = 12.0f;
    m_windowConfig.styles.appliedColorStyles = 0;
    m_windowConfig.styles.appliedVarStyles   = 0;
    m_windowConfig.flags = ImGuiWindowFlags_NoTitleBar
                         | ImGuiWindowFlags_NoResize
                         | ImGuiWindowFlags_NoMove
                         | ImGuiWindowFlags_NoScrollbar
                         | ImGuiWindowFlags_NoCollapse;;
}

void ObjectManipulationLayer::onImGuiRender()
{
    ui::Window::setPosAndSize(m_windowPosConfig);
    if (!VisibilityHandler::isVisible("OBJECT_MANIPULATION_LAYER"))
    {
        return;
    }

    ui::Window::init(m_windowConfig);

    ui::Window::addToLayout(0.0f, [this]() {
        float iconSize = ImGui::GetMainViewport()->WorkSize.x * 0.015f;

        for (auto& button : m_buttons)
        {
            if (ImGui::ImageButton(button->name().c_str(), button->textureID(), { iconSize, iconSize }))
            {
                //TODO
            }
        }
    });
    
    ui::Window::destroy(m_windowConfig);
}
