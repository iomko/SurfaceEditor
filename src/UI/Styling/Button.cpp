#include "Button.h"
#include <iostream>

namespace ui::styling
{

    void Button::init(ui::components::Button* button, ButtonConfig* config)
    {
        auto* radioButton = dynamic_cast<ui::components::RadioButton*>(button);
        if (radioButton != nullptr)
        {
            if (radioButton->isSelected())
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ui::styling::Color::selectedOrange);
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_Button, ui::styling::Color::black);
            }
        }
        else
        {
            ImGui::PushStyleColor(ImGuiCol_Button, config->background);
        }
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
            float scale  = std::min(scaleX, scaleY);

            ImGui::SetWindowFontScale(scale);
        }

        config->styles.appliedColorStyles += 3;
        config->styles.appliedVarStyles   += 2;
    }

    void Button::destroy(ButtonConfig* config)
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

    void Button::onRadioButtonClick(ui::components::RadioButton* button)
    {
        for (auto& groupButton : button->group())
        {
            groupButton->setIsSelected(false);
        }
        button->setIsSelected(true);
    }

} // ui::styling