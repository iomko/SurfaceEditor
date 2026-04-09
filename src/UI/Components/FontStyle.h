#pragma once
#include <imgui.h>

class FontStyle
{
public:
    FontStyle() = delete;

    ~FontStyle() = delete;

    static void init(ImGuiIO& io);

    static void headliner();

    static void regular();

    static void end();

private:
    inline static ImFont* s_regular     = nullptr;
    inline static ImFont* s_bold        = nullptr;
    inline static ImFont* s_extraBold   = nullptr;
};