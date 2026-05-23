#include "Button.h"
#include "../Components/Button.h"
#include <iostream>

namespace ui::styling
{

    namespace
    {

        void init(ButtonConfig* config)
        {
            ImGui::PushStyleColor(ImGuiCol_Button, config->background);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, config->onHoverOverColor);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, config->onClickColor);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, config->rounding);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, config->framePadding);

            const ImVec2 windowSize = ImGui::GetMainViewport()->Size;

            float realSizeX = windowSize.x * config->size.x;
            float realSizeY = windowSize.y * config->size.y;

            if (config->square)
            {
                const float squaredSize = std::min(realSizeX, realSizeY);

                realSizeX = squaredSize;
                realSizeY = squaredSize;
            }

            config->realSize = ImVec2{realSizeX, realSizeY};

            if (config->font != nullptr)
            {
                ImGui::PushFont(config->font);

                ImVec2 textSize = ImGui::CalcTextSize("Aa");

                float scaleX = ImGui::GetFontSize() * (config->realSize.x / textSize.x);
                float scaleY = ImGui::GetFontSize() * (config->realSize.y / textSize.y);

                float scale = std::min(scaleX, scaleY);

                ImGui::SetWindowFontScale(scale);
            }

            config->styles.appliedColorStyles += 3;
            config->styles.appliedVarStyles += 2;
        }

        void destroy(ButtonConfig* config)
        {
            if (config->font != nullptr)
            {
                ImGui::SetWindowFontScale(1.0f);
                ImGui::PopFont();
            }

            ImGui::PopStyleColor(config->styles.appliedColorStyles);
            ImGui::PopStyleVar(config->styles.appliedVarStyles);

            config->styles.appliedColorStyles = 0;
            config->styles.appliedVarStyles = 0;
        }

        void draw(ui::components::Button* button, ButtonConfig* config)
        {
            const ImVec2 buttonSize = config->realSize;

            if (ImGui::Button(button->name().c_str(), buttonSize))
            {
                button->execute();
            }
        }

    } // namespace

    void Button::render(ui::components::Button* button)
    {
        auto* config = dynamic_cast<ButtonConfig*>(button->config());

        init(config);
        draw(button, config);
        destroy(config);
    }

} // ui::styling