#pragma once
#include <string>
#include "Common.h"

namespace ui::styling
{

    template<typename T>
    struct SliderConfig : IConfig
    {
        T      min;
        T      max;
        float  width;
        ImVec4 background;
        ImVec4 onHoverBackground;
        ImVec4 onActiveBackground;
        ImVec4 grabBackground;
        ImVec4 onGrabActiveBackground;
    };

    class Slider
    {
    public:
        Slider() = delete;

        template<typename T>
        static void render(ui::components::Slider<T>* component);

    private:
        template<typename T>
        static void initConfig(SliderConfig<T>* config)
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, config->background);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, config->onHoverBackground);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, config->onActiveBackground);
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, config->grabBackground);
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, config->onGrabActiveBackground);
            ImGui::SetNextItemWidth(config->width * ImGui::GetWindowSize().x);

            config->styles.appliedColorStyles += 5;
        }

        template<typename T>
        static void destroy(SliderConfig<T>* config)
        {
            ImGui::PopStyleColor(config->styles.appliedColorStyles);

            config->styles.appliedColorStyles = 0;
        }
    };

    template<typename T>
    void Slider::render(ui::components::Slider<T>*)
    {
        throw std::logic_error("Unsupported type for slider");
    }

    template<>
    void Slider::render<int>(ui::components::Slider<int>* slider)
    {
        auto* config = dynamic_cast<SliderConfig<int>*>(slider->config());

        initConfig(config);

        ImGui::SliderInt(slider->name().c_str(), slider->inputValue(), config->min, config->max);

        destroy(config);
    }

    template<>
    void Slider::render<float>(ui::components::Slider<float>* slider)
    {
        auto* config = dynamic_cast<SliderConfig<float>*>(slider->config());

        initConfig(config);

        ImGui::SliderFloat(slider->name().c_str(), slider->inputValue(), config->min, config->max);

        destroy(config);
    }

} // ui::comopnents