#include "Label.h"
#include "../Components/Label.h"

namespace ui::styling
{

    namespace
    {

        void init(ui::components::Label* label, LabelConfig* config)
        {
            const ImVec2 windowSize = ImGui::GetMainViewport()->Size;

            ImGui::PushFont(config->font);

            ImVec2 textSize = ImGui::CalcTextSize("Aa");

            float scaleX = ImGui::GetFontSize() * (windowSize.x / textSize.x);
            float scaleY = ImGui::GetFontSize() * (windowSize.y / textSize.y);

            float scale = std::min(scaleX, scaleY);

            ImGui::SetWindowFontScale(scale);

            ImGui::TextColored(config->color, "%s", label->name().c_str());
        }

        void destroy(LabelConfig* config)
        {
            ImGui::SetWindowFontScale(1.0f);
            ImGui::PopFont();
        }

    } // namespace

    void Label::render(ui::components::Label* label)
    {
        auto* config = dynamic_cast<LabelConfig*>(label->config());

        init(label, config);
        destroy(config);
    }

} // ui::styling