#pragma once
#include  <imgui.h>
#include <glm/glm.hpp>
#include "Components/Window.h"

class OverlappingWindow
{
public:
    virtual ~OverlappingWindow() = default;

    bool clickedOnWindow(const glm::vec2& clickPos);

protected:
    virtual void initWindowPosConfig() = 0;

    virtual void initWindowConfig() = 0;

protected:
    ui::WindowPosConfig m_windowPosConfig;
    ui::WindowConfig    m_windowConfig;
};