#include "WindowStyle.h"

void WindowStyle::setDefaultTitleBar(int& appliedColorStyles)
{
    ImVec4 titleColor = ImVec4(1.00f, 0.45f, 0.00f, 1.0f);
    ImVec4 titleColorActive = ImVec4(1.00f, 0.45f, 0.00f, 1.0f);
    ImVec4 titleColorCollapsed = ImVec4(1.0f, 0.65f, 0.25f, 0.8f);

    ImGui::PushStyleColor(ImGuiCol_TitleBg, titleColor);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, titleColorActive);
    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, titleColorCollapsed);

    appliedColorStyles += 3;
}

void WindowStyle::checkResolutionRange(LayerIDS layer, float height, float width, float minHeight, float minWidth)
{
    static constexpr const int fullWidth = 1920;
    static constexpr const int fullHeight = 1080;
    const int minResWidth = fullWidth * minWidth;
    const int minResHeight = fullHeight * minHeight;

    if (width < minResWidth || height < minResHeight)
    {
        VisibilityHandler::setOutOfRange(layer);
    }
    else
    {
        VisibilityHandler::setInRange(layer);
    }
}

void WindowStyle::drawHorizontalSeparator(float length)
{
    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::GetWindowDrawList()->AddLine(
        pos,
        ImVec2(pos.x + length, pos.y),
        ImGui::GetColorU32(ImGuiCol_Separator),
        1.0f
    );
}

void WindowStyle::setup(
    const char* name,
    const ImGuiWindowFlags flags,
    int& appliedColorStyles,
    int& appliedVarStyles,
    bool transparent
)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
    if (!transparent)
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.17f, 0.85f));
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.17f, 0.70f));
    }
    ImGui::Begin(name, nullptr, flags);

    ++appliedColorStyles;
    ++appliedVarStyles;
}

void WindowStyle::end(int appliedColorStyles, int appliedVarStyles)
{
    ImGui::End();
    ImGui::PopStyleColor(appliedColorStyles);
    ImGui::PopStyleVar();
}