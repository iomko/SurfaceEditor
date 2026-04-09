#include "CheckBoxStyle.h"

void CheckBoxStyle::basic(int& styleColorsApplied, int& styleVarsApplied, float padding)
{
    ImGui::PushStyleColor(ImGuiCol_CheckMark, s_checkOrange);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, s_bgGray);
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, s_bgGray);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, s_bgGrayActive);

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding, padding));
    
    styleColorsApplied += 4;
    ++styleVarsApplied;
}

void CheckBoxStyle::end(int styleColorsApplied, int styleVarsApplied)
{
    ImGui::PopStyleColor(styleColorsApplied);
    ImGui::PopStyleVar(styleVarsApplied);
}