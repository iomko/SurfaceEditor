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
        static void render(std::string& name, T* value, IConfig* config);

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
    inline void Slider::render(std::string& name, T* value, IConfig* config)
    {
        throw std::logic_error("Unsupported type for slider");
    }

    template<>
    inline void Slider::render<int>(std::string& name, int* value, IConfig* config)
    {
        auto* sliderConfig = dynamic_cast<SliderConfig<int>*>(config);

        initConfig(sliderConfig);

        ImGui::SliderInt(name.c_str(), value, sliderConfig->min, sliderConfig->max);

        destroy(sliderConfig);
    }

    template<>
    inline void Slider::render<float>(std::string& name, float* value, IConfig* config)
    {
        auto* sliderConfig = dynamic_cast<SliderConfig<float>*>(config);

        initConfig(sliderConfig);

        ImGui::SliderFloat(name.c_str(), value, sliderConfig->min, sliderConfig->max);

        destroy(sliderConfig);
    }

} // ui::comopnents