#include "SculptToolsLayer.h"
#include "imgui.h"
#include "../ViewPortsController.h"
#include "../Tools/ToolRegistry.h"
#include "../Tools/ToolIDs.h"

static AutoRegisterLayerArgs<SculptToolsLayer, std::string> reg;

SculptToolsLayer::SculptToolsLayer(const std::string &name)
    : LayerWithID(name) {}

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

void SculptToolsLayer::onImGuiRender()
{
    ImGui::Begin(this->getName().c_str());

    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 mousePos = ImGui::GetMousePos();

    m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
                             mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);


    auto *brushTool = ToolRegistry::instance().getTool(BRUSH_TOOL);
    if(brushTool)
    {
        if (ImGui::TreeNode("BrushTool"))
        {
            if (ImGui::Button("Choose Brush Tool"))
            {
                std::cout << "Chosen Brush Tool" << std::endl;
                ViewPortsHolderContext::s_viewPortsController->m_currentTool = brushTool;
                if (ViewPortsHolderContext::s_viewPortsController->m_currentToolParams != nullptr)
                {
                    delete ViewPortsHolderContext::s_viewPortsController->m_currentToolParams;
                    ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = nullptr;
                }
                BrushToolParams *brushToolParams = new BrushToolParams();
                brushToolParams->brushStrength = m_brushStrength;
                brushToolParams->radius = m_brushRadius;
                ViewPortsHolderContext::s_viewPortsController->m_currentToolParams = brushToolParams;
            }

            ImGui::SliderFloat("BrushStrength", &m_brushStrength, -50.0f, 50.0f, "%.1f");
            ImGui::SliderFloat("BrushRadius", &m_brushRadius, 0.0f, 50.0f, "%.1f");

            ImGui::TreePop();
        }
    }
    

    ImGui::End();
}