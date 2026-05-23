#include "InputBox.h"

namespace ui::styling
{

    void InputBox::initConfig(IConfig* config)
    {
        auto* inputBoxConfig = dynamic_cast<InputBoxConfig*>(config);

        ImGui::PushStyleColor(ImGuiCol_FrameBg, inputBoxConfig->background);
        ImGui::SetNextItemWidth(inputBoxConfig->width * ImGui::GetWindowSize().x);

        ++inputBoxConfig->styles.appliedColorStyles;
    }

    void InputBox::destroy(IConfig* config)
    {
        auto* inputBoxConfig = dynamic_cast<InputBoxConfig*>(config);

        ImGui::PopStyleColor(inputBoxConfig->styles.appliedColorStyles);

        inputBoxConfig->styles.appliedColorStyles = 0;
    }

} // ui::styling