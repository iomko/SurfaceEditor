#pragma once
#include  <imgui.h>
#include <glm/glm.hpp>

class OverlappingWindow
{
public:
    bool clickedOnWindow(const glm::vec2& clickPos);

protected:
    ImVec2 m_pos;
    ImVec2 m_size;
};