#include "SliderStyle.h"

void SliderStyle::basic(int& styleColorsApplied, float width)
{
    ImGui::SetNextItemWidth(width);

    ImGui::PushStyleColor(ImGuiCol_FrameBg, s_bgGray);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, s_bgGray);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, s_bgGrayActive);
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, s_orange);
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(1.0f, 0.5f, 0.1f, 1.0f));

    styleColorsApplied += 5;
}

void SliderStyle::end(int& styleColorsApplied)
{
    ImGui::PopStyleColor(styleColorsApplied);
    
    styleColorsApplied = 0;
}