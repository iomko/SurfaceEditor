#include "Button.h"

namespace ui::styling
{

    void Button::init(ButtonConfig& config)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, config.background);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, config.onHoverOverColor);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, config.onClickColor);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, config.rounding);

        config.styles.appliedColorStyles += 3;
        ++config.styles.appliedVarStyles;
    }

    void Button::destroy(ButtonConfig& config)
    {
        ImGui::PopStyleColor(config.styles.appliedColorStyles);
        ImGui::PopStyleVar(config.styles.appliedVarStyles);

        config.styles.appliedColorStyles = 0;
        config.styles.appliedVarStyles   = 0;
    }

    void Button::changeBackgroundColor(ButtonConfig& config)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, config.background);

        ++config.styles.appliedColorStyles;
    }

} // ui::styling