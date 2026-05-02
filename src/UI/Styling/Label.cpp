#include "Label.h"

namespace ui::styling
{

    void Label::init(const std::string& text, const LabelConfig& config)
    {
        ImGui::PushFont(config.font);
        ImGui::TextColored(config.color, text.c_str());
    }

    void Label::destroy(const LabelConfig& config)
    {
        ImGui::PopFont();
    }

} // ui::styling