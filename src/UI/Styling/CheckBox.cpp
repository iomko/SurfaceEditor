#include "CheckBox.h"

namespace ui::styling
{

    void CheckBox::init(const std::string& name, bool* isChecked, CheckBoxConfig& config)
    {
        ImGui::PushStyleColor(ImGuiCol_CheckMark, config.checkMarkColor);
        ImGui::PushStyleColor(ImGuiCol_FrameBg, config.background);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, config.hoveredBackground);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, config.activeBackground);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, config.framePadding);

        std::string checkBoxName = "##" + name;

        ImGui::Checkbox(checkBoxName.c_str(), isChecked);

        config.styles.appliedColorStyles += 4;
        ++config.styles.appliedVarStyles;
    }

    void CheckBox::destroy(CheckBoxConfig& config)
    {
        ImGui::PopStyleColor(config.styles.appliedColorStyles);
        ImGui::PopStyleVar(config.styles.appliedVarStyles);

        config.styles.appliedColorStyles = 0;
        config.styles.appliedVarStyles   = 0;
    }

} // ui::styling