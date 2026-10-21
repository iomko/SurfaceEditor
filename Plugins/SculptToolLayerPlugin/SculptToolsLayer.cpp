#include "SculptToolsLayer.h"
#include "imgui.h"
#include "../../src/ViewPortsController.h"
#include "../../src/Tools/ToolRegistry.h"
#include <Builders/UIWindowBuilder.h>
#include <Builders/UIButtonBuilder.h>
#include <Builders/UICollapsingHeaderBuilder.h>
#include <Builders/UIDragBuilder.h>
#include <Styling/Font.h>
#include <Components/CollapsingHeader.h>
#include <Components/Button.h>
#include <Components/Drag.h>

namespace
{
    static constexpr const char *LAYER_NAME = "SCULPT_TOOLS_LAYER";
}

static AutoRegisterLayerArgs<SculptToolsLayer, std::string> reg("SCULPT_TOOLS_LAYER");

SculptToolsLayer::SculptToolsLayer(const std::string &name)
    : Layer(name), ui::components::Window(LAYER_NAME) 
    {
        VisibilityHandler::show(LAYER_NAME);
    }

void SculptToolsLayer::onEvent(Event &event)
{
    if (event.getType() == EventType::MouseButtonPress)
    {
        if (m_isMouseInsideWindow)
        {
            event.isHandled = true;
        }
    }
}

void SculptToolsLayer::initWindowConfig()
{
    auto posConfig = WindowPosConfigBuilder()
                         .posX(0)
                         .posY(0.15f)
                         .build();

    auto sizeConfig = WindowSizeConfigBuilder()
                          .width(0.3f)
                          .height(0.3f)
                          .minWidth(0.01f)
                          .minHeight(0.01f)
                          .build();

    auto titleBarConfig = WindowTitleBarBuilder()
                              .background(ui::styling::Color::titleBarOrange)
                              .font(ui::styling::Font::extraBold(20.0f))
                              .build();


    m_windowConfig = WindowConfigBuilder()
                         .name("Sculpt tools layer")
                         .layerName(LAYER_NAME)
                         .background(ui::styling::Color::transparentGray)
                         .titleBar(titleBarConfig)
                         .pos(posConfig)
                         .size(sizeConfig)
                         .build();
}

void SculptToolsLayer::initComponents()
{
    auto btnConfig = ButtonConfigBuilder()
                         .rounding(8.0f)
                         .background(ui::styling::Color::lightGray)
                         .font(ui::styling::Font::regular(0.6f))
                         .size(ImVec2{0.1f, 0.04f})
                         .onHoverColor(ui::styling::Color::hoverOverOrange)
                         .onClickColor(ui::styling::Color::onClickOrange)
                         .build();

    auto collapsingHeaderConfig = CollapsingHeaderConfigBuilder().build();

    auto firstDragConfig = DragConfigBuilder<float>()
                                .min(-50.0f)
                                .max(50.0f)
                                .width(0.5f)
                                .background(ui::styling::Color::lightGray)
                                .frameBorderSize(1.0f)
                                .build();

    auto secondDragConfig = DragConfigBuilder<float>()
                                .min(0.0f)
                                .max(50.0f)
                                .width(0.5f)
                                .background(ui::styling::Color::lightGray)
                                .frameBorderSize(1.0f)
                                .build();
                            

    auto layout = emplaceLayout(ImVec2{0.01f, 0.01f});
    layout->reserveComponents(1);
    m_collapsingHeader = emplaceComponent<ui::components::CollapsingHeader>(layout,"BrushTool",collapsingHeaderConfig);
    auto collapsingHeaderLayout = m_collapsingHeader->emplaceLayout(ImVec2{0.01f, 0.01f});
    collapsingHeaderLayout->reserveComponents(3);
    m_collapsingHeader->emplaceComponent<ui::components::Button>(collapsingHeaderLayout, "Choose brush tool", btnConfig ,[this](ui::components::Button *button)
        {
            std::cout << "Chosen Brush Tool" << std::endl;
            ViewPortsHolderContext::s_viewPortsController->m_currentTool = m_brushTool;
            if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
            {
                delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
                ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
            }
            BrushToolParams *brushToolParams = new BrushToolParams();
            brushToolParams->brushStrength = m_brushStrength;
            brushToolParams->radius = m_brushRadius;
            ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = brushToolParams; 
        });
    m_dragStrength = m_collapsingHeader->emplaceComponent<ui::components::Drag<float>>(collapsingHeaderLayout,"BrushStrength",firstDragConfig);
    m_dragRadius = m_collapsingHeader->emplaceComponent<ui::components::Drag<float>>(collapsingHeaderLayout, "BrushRadius",secondDragConfig);
    
}
void SculptToolsLayer::onImGuiRender()
{
    this->render();

    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 mousePos = ImGui::GetMousePos();

    m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                             mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);

    m_brushTool = ToolRegistry::instance().getTool("BRUSH_TOOL");
    if (m_brushTool)
    {
        m_collapsingHeader->setIsVisible(true);
        m_brushStrength = *m_dragStrength->inputValue();
        m_brushRadius = *m_dragRadius->inputValue();
    } else {
        m_collapsingHeader->setIsVisible(false);
    }
}