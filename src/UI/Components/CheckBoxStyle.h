#pragma once
#include <imgui.h>

class CheckBoxStyle
{
public:
    CheckBoxStyle() = delete;

    ~CheckBoxStyle() = delete;

    static void basic(int& styleColorsApplied, int& styleVarsApplied, float padding);

    static void end(int styleColorsApplied, int styleVarsApplied);

private:
    inline static ImVec4 s_checkOrange  = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
    inline static ImVec4 s_bgGray       = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
    inline static ImVec4 s_bgGrayActive = ImVec4(0.05f, 0.05f, 0.05f, 1.0f);
};