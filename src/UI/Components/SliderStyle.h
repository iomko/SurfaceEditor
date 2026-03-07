#pragma once
#include <imgui.h>

class SliderStyle
{
public:
    SliderStyle() = delete;

    ~SliderStyle() = delete;

    static void basic(int& styleColorsApplied, float width);

    static void end(int& styleColorsApplied);

private:
    inline static ImVec4 s_orange       = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
    inline static ImVec4 s_bgGray       = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    inline static ImVec4 s_bgGrayActive = ImVec4(0.15f, 0.15f, 0.15f, 1.0f);
};