#include "InputBox.h"

namespace ui::styling
{

    void InputBox::initConfig(InputBoxConfig& config)
    {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, config.background);
        ImGui::SetNextItemWidth(config.width * ImGui::GetWindowSize().x);

        ++config.styles.appliedColorStyles;
    }

    void InputBox::destroy(InputBoxConfig& config)
    {
        ImGui::PopStyleColor(config.styles.appliedColorStyles);

        config.styles.appliedColorStyles = 0;
    }

} // ui::styling