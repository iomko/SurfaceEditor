#pragma once
#include "Common.h"
#include "../Components/Common.h"
#include "../Components/RadioImageButton.h"

namespace ui::styling
{

    struct ButtonConfig : IConfig
    {
        bool    square;
        float   rounding;
        ImFont* font;
        ImVec2  size;
        ImVec2  realSize;
        ImVec4  background;
        ImVec4  onHoverOverColor;
        ImVec4  onClickColor;
        ImVec2  framePadding;
    };

    class Button
    {
    public:
        Button() = delete;

        template<ui::ButtonType T = ui::ButtonType::DEFAULT>
        static void render(ui::components::Button* button)
        {
            auto* config = dynamic_cast<ButtonConfig*>(button->config());

            init(button, config);
            draw<T>(button, config);
            destroy(config);
        }

    private:
        template<ui::ButtonType T>
        static void draw(ui::components::Button* button, ButtonConfig* config);

        static void onRadioButtonClick(ui::components::RadioButton* button);

        static void init(ui::components::Button* button, ButtonConfig* config);

        static void destroy(ButtonConfig* config);
    };

    template<ui::ButtonType T>
    inline void Button::draw(ui::components::Button* button, ButtonConfig* config)
    {
        throw std::runtime_error("Unknown button type");
    }

    template<>
    inline void Button::draw<ui::ButtonType::DEFAULT>(ui::components::Button* button, ButtonConfig* config)
    {
        const ImVec2 buttonSize = config->realSize;

        if (ImGui::Button(button->name().c_str(), buttonSize))
        {
            button->execute();
        }
    }

    template<>
    inline void Button::draw<ui::ButtonType::RADIO>(ui::components::Button* button, ButtonConfig* config)
    {
        auto* radioButton = dynamic_cast<ui::components::RadioButton*>(button);

        const ImVec2 buttonSize = config->realSize;

        if (ImGui::Button(radioButton->name().c_str(), buttonSize))
        {
            onRadioButtonClick(radioButton);
            radioButton->execute();
        }
    }

    template<>
    inline void Button::draw<ui::ButtonType::IMAGE>(ui::components::Button* button, ButtonConfig* config)
    {
        auto* imageButton = dynamic_cast<ui::components::ImageButton*>(button);

        const ImVec2 buttonSize = config->realSize;

        if (ImGui::ImageButton(button->name().c_str(), imageButton->textureID(), buttonSize))
        {
            button->execute();
        }
    }

    template<>
    inline void Button::draw<ui::ButtonType::RADIO_IMAGE>(ui::components::Button* button, ButtonConfig* config)
    {
        auto* radioImageButton = dynamic_cast<ui::components::RadioImageButton*>(button);

        const ImVec2 buttonSize = config->realSize;

        if (ImGui::ImageButton(radioImageButton->name().c_str(), radioImageButton->textureID(), buttonSize))
        {
            onRadioButtonClick(radioImageButton);
            radioImageButton->execute();
        }
    }

} // ui::styling