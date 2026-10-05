#include "OverlappingWindow.h"

bool OverlappingWindow::clickedOnWindow(const glm::vec2& clickPos)
{
    ImVec2 bottomRight = ImVec2(m_pos.x + m_size.x, m_pos.y + m_size.y);
    
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 pos = viewport->Pos;

    ImVec2 actualClickPos(pos.x + clickPos.x, pos.y + clickPos.y); 

    return actualClickPos.x >= m_pos.x && actualClickPos.x <= bottomRight.x &&
           actualClickPos.y >= m_pos.y && actualClickPos.y <= bottomRight.y;
}