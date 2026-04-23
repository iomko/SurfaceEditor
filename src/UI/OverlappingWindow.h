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
    void setupWindow();

    virtual void initWindowPosConfig() = 0;

    virtual void initWindowSizeConfig() = 0;

    virtual void initWindowConfig() = 0;

protected:
    ui::WindowConfig m_windowConfig;
};