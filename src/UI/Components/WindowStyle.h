#pragma once
#include <string>
#include <imgui.h>

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

    static void setup(const char* name, const ImGuiWindowFlags flags);

    static void end();
};