#pragma once
#include "../UI/Styling/Slider.h"

template<typename T>
class SliderConfigBuilder
{
public:
    SliderConfigBuilder()
        : m_config{}
    {
        m_config.min = 0;
        m_config.max = 10'000;
    }

    SliderConfigBuilder& min(T min)
    {
        m_config.min = min;
        return *this;
    }

    SliderConfigBuilder& max(T max)
    {
        m_config.max = max;
        return *this;
    }

    SliderConfigBuilder& width(float width)
    {
        m_config.width = width;
        return *this;
    }

    SliderConfigBuilder& background(const ImVec4& background)
    {
        m_config.background = background;
        return *this;
    }

    SliderConfigBuilder& onHoverBackground(const ImVec4& background)
    {
        m_config.onHoverBackground = background;
        return *this;
    }

    SliderConfigBuilder& onActiveBackground(const ImVec4& background)
    {
        m_config.onActiveBackground = background;
        return *this;
    }

    SliderConfigBuilder& grabBackground(const ImVec4& background)
    {
        m_config.grabBackground = background;
        return *this;
    }

    SliderConfigBuilder& onGrabActiveBackground(const ImVec4& background)
    {
        m_config.onGrabActiveBackground = background;
        return *this;
    }

    ui::styling::SliderConfig<T>&& build()
    {
        return std::move(m_config);
    }

private:
    ui::styling::SliderConfig<T> m_config;
};