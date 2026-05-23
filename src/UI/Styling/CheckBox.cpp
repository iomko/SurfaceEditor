#include "CheckBox.h"
#include "../Components/CheckBox.h"

namespace ui::styling
{

    namespace
    {

        void init(ui::components::CheckBox* checkBox, CheckBoxConfig* config)
        {
            ImGui::PushStyleColor(ImGuiCol_CheckMark, config->checkMarkColor);
            ImGui::PushStyleColor(ImGuiCol_FrameBg, config->background);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, config->hoveredBackground);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, config->activeBackground);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, config->framePadding);

            std::string checkBoxName = "##" + checkBox->name();

            ImGui::Checkbox(checkBoxName.c_str(), checkBox->isChecked());

            config->styles.appliedColorStyles += 4;
            ++config->styles.appliedVarStyles;
        }

        void destroy(CheckBoxConfig* config)
        {
            ImGui::PopStyleColor(config->styles.appliedColorStyles);
            ImGui::PopStyleVar(config->styles.appliedVarStyles);

            config->styles.appliedColorStyles = 0;
            config->styles.appliedVarStyles = 0;
        }

    } // namespace

    void CheckBox::render(ui::components::CheckBox* checkBox)
    {
        auto* config = dynamic_cast<CheckBoxConfig*>(checkBox->config());

        init(checkBox, config);
        destroy(config);
    }

} // ui::styling