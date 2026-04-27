#pragma once
#include  <imgui.h>
#include <glm/glm.hpp>
#include "Styling/Window.h"

class OverlappingWindow
{
public:
    OverlappingWindow(ui::styling::WindowConfig config);

    virtual ~OverlappingWindow() = default;

    bool clickedOnWindow(const glm::vec2& clickPos);

protected:
    virtual ui::styling::WindowConfig initWindowConfig() = 0;

protected:
    ui::styling::WindowConfig m_windowConfig;
};