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

void ButtonStyle::simplePopUpWindowStyle(float buttonHeight, int& appliedColorStyles, int& appliedVarStyles)
{
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(12, buttonHeight * 0.18f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6, 6));

    appliedVarStyles += 3;

    ImGui::PushStyleColor(ImGuiCol_Button,        ImVec4(0.20f, 0.20f, 0.23f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.00f, 0.55f, 0.00f, 0.85f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  ImVec4(1.00f, 0.45f, 0.00f, 1.0f));

    appliedColorStyles += 3;
}

void ButtonStyle::applyResponsiveFontScale(float buttonHeight, float baseFontSize)
{
    float paddingY = ImGui::GetStyle().FramePadding.y * 2.0f;
    float usableHeight = buttonHeight - paddingY;

    float scale = usableHeight / baseFontSize;

    scale = std::clamp(scale, 0.8f, 3.0f);

    ImGui::SetWindowFontScale(scale);
}

ButtonLayout ButtonStyle::calculateVerticalButtonLayout(int buttonCount)
{
    float availableWidth  = ImGui::GetContentRegionAvail().x;
    float availableHeight = ImGui::GetContentRegionAvail().y;

    float spacing = ImGui::GetStyle().ItemSpacing.y;
    float totalSpacing = spacing * (buttonCount - 1);

    float buttonHeight = (availableHeight - totalSpacing) / buttonCount;
    float buttonWidth  = availableWidth;

    return { buttonWidth, buttonHeight };
}

void ButtonStyle::closeStyling(int appliedColorStyles, int appliedVarStyles)
{
    ImGui::PopStyleColor(appliedColorStyles);
    ImGui::PopStyleVar(appliedVarStyles);
    ImGui::SetWindowFontScale(1.0f);
}