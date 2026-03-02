#include "ButtonStyle.h"

void ButtonStyle::setRadioButtonBackground(RadioButton* button,  int& appliedColorStyles)
{
    if (button->isSelected())
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.65f, 0.10f, 0.55f));
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
    }

    ++appliedColorStyles;
}

void ButtonStyle::controlPanelStyle(int& appliedColorStyles, int& appliedVarStyles)
{
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.55f, 0.0f, 0.35f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.55f, 0.0f, 0.55f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 20.0f);

    appliedColorStyles += 2;
    ++appliedVarStyles;
}

void ButtonStyle::closeStyling(int appliedColorStyles, int appliedVarStyles)
{
    ImGui::PopStyleColor(appliedColorStyles);
    ImGui::PopStyleVar(appliedVarStyles);
}