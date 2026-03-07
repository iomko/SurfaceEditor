#pragma once
#include <string>
#include <imgui.h>
#include "../VisibilityHandler.h"

class WindowStyle
{
public:
    WindowStyle() = delete;
    
    ~WindowStyle() = delete;

    inline static ImGuiWindowFlags defaultWindow()
    {
        return ImGuiWindowFlags_NoTitleBar
            | ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoScrollbar
            | ImGuiWindowFlags_NoCollapse;
    }

    inline static ImGuiWindowFlags windowWithTitleBar()
    {
        return ImGuiWindowFlags_NoResize
            | ImGuiWindowFlags_NoMove
            | ImGuiWindowFlags_NoScrollbar
            | ImGuiWindowFlags_NoCollapse;
    }

    static void setDefaultTitleBar(int& appliedColorStyles);

    static void checkResolutionRange(LayerIDS layer, float height, float width, float minHeight = 0.6f, float minWidth = 0.6f);

    static void setup(
        const char* name,
        const ImGuiWindowFlags flags,
        int& appliedColorStyles,
        int& appliedVarStyles
    );

    static void end(int appliedColorStyles, int appliedVarStyles);
};