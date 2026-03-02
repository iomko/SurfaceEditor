#include "WindowStyle.h"

void WindowStyle::setup(const char* name, const ImGuiWindowFlags flags)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.17f, 0.85f));
    ImGui::Begin(name, nullptr, flags);
}

void WindowStyle::end()
{
    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
}