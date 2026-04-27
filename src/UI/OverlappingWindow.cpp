#include "OverlappingWindow.h"
#include "../ViewPortsController.h"

OverlappingWindow::OverlappingWindow(ui::styling::WindowConfig config)
    : m_windowConfig(std::move(config))
{
    ViewPortsHolderContext::s_uiLayerController->registerUiWindow(this);
}

bool OverlappingWindow::clickedOnWindow(const glm::vec2& clickPos)
{
    ImVec2 bottomRight = ImVec2(
        m_windowConfig.pos.rawPos.x + m_windowConfig.size.rawSize.x,
        m_windowConfig.pos.rawPos.y + m_windowConfig.size.rawSize.y
    );
    
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 pos = viewport->Pos;

    ImVec2 actualClickPos(pos.x + clickPos.x, pos.y + clickPos.y); 

    return actualClickPos.x >= m_windowConfig.pos.rawPos.x && actualClickPos.x <= bottomRight.x &&
           actualClickPos.y >= m_windowConfig.pos.rawPos.y && actualClickPos.y <= bottomRight.y;
}