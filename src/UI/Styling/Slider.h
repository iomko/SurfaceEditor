#pragma once
#include <string>
#include "Common.h"

namespace ui::styling
{

    template<typename T>
    struct SliderConfig
    {
        T              min;
        T              max;
        float          width;
        ImVec4         background;
        ImVec4         onHoverBackground;
        ImVec4         onActiveBackground;
        ImVec4         grabBackground;
        ImVec4         onGrabActiveBackground;
        AppliedStyling styles;
    };

    class Slider
    {
    public:
        Slider() = delete;

        ~Slider() = delete;

        template<typename T>
        static void init(const std::string& name, T* input, SliderConfig<T>& config);

        template<typename T>
        static void destroy(SliderConfig<T>& config)
        {
            ImGui::PopStyleColor(config.styles.appliedColorStyles);

            config.styles.appliedColorStyles = 0;
        }

    private:
        template<typename T>
        static void initConfig(SliderConfig<T>& config)
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, config.background);
            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, config.onHoverBackground);
            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, config.onActiveBackground);
            ImGui::PushStyleColor(ImGuiCol_SliderGrab, config.grabBackground);
            ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, config.onGrabActiveBackground);
            ImGui::SetNextItemWidth(config.width * ImGui::GetWindowSize().x);

            config.styles.appliedColorStyles += 5;
        }
    };

    template<typename T>
    void Slider::init(const std::string& name, T* input, SliderConfig<T>& config)
    {
        throw std::logic_error("Unsupported type for slider");
    }

    template<>
    void Slider::init<int>(const std::string& name, int* input, SliderConfig<int>& config)
    {
        initConfig(config);

        ImGui::SliderInt(name.c_str(), input, config.min, config.max);
    }

    template<>
    void Slider::init<float>(const std::string& name, float* input, SliderConfig<float>& config)
    {
        initConfig(config);

        ImGui::SliderFloat(name.c_str(), input, config.min, config.max);
    }

} // ui::comopnents