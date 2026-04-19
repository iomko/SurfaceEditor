#include "OverlappingWindow.h"

bool OverlappingWindow::clickedOnWindow(const glm::vec2& clickPos)
{
    ImVec2 bottomRight = ImVec2(
        m_windowPosConfig.rawPos.x + m_windowPosConfig.rawSize.x,
        m_windowPosConfig.rawPos.y + m_windowPosConfig.rawSize.y
    );
    
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 pos = viewport->Pos;

    ImVec2 actualClickPos(pos.x + clickPos.x, pos.y + clickPos.y); 

    return actualClickPos.x >= m_windowPosConfig.rawPos.x && actualClickPos.x <= bottomRight.x &&
           actualClickPos.y >= m_windowPosConfig.rawPos.y && actualClickPos.y <= bottomRight.y;
}