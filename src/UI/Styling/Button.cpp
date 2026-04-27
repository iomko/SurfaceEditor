#include "Button.h"

namespace ui::styling
{

    void Button::init(ButtonConfig& config)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, config.background);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, config.onHoverOverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, config.onClickColor);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, config.rounding);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, config.framePadding);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, config.windowPadding);

        config.styles.appliedColorStyles += 3;
        config.styles.appliedVarStyles   += 3;
    }

    void Button::destroy(ButtonConfig& config)
    {
        ImGui::PopStyleColor(config.styles.appliedColorStyles);
        ImGui::PopStyleVar(config.styles.appliedVarStyles);

        config.styles.appliedColorStyles = 0;
        config.styles.appliedVarStyles   = 0;
    }

} // ui::styling