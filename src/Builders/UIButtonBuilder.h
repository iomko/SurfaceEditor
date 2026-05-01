#pragma once
#include <utility>
#include "../UI/Styling/Button.h"

class ButtonConfigBuilder
{
public:
    ButtonConfigBuilder()
        : m_config{}
    { }

    ButtonConfigBuilder& rounding(float rounding)
    {
        m_config.rounding = rounding;
        return *this;
    }

    ButtonConfigBuilder& font(ImFont* font)
    {
        m_config.font = font;
        return *this;
    }

    ButtonConfigBuilder& size(const ImVec2& size)
    {
        m_config.size = size;
        return *this;
    }

    ButtonConfigBuilder& background(const ImVec4& background)
    {
        m_config.background = background;
        return *this;
    }

    ButtonConfigBuilder& onHoverColor(const ImVec4& onHoverColor)
    {
        m_config.onHoverOverColor = onHoverColor;
        return *this;
    }

    ButtonConfigBuilder& onClickColor(const ImVec4& onClickColor)
    {
        m_config.onClickColor = onClickColor;
        return *this;
    }

    ButtonConfigBuilder& framePadding(const ImVec2& padding)
    {
        m_config.framePadding = padding;
        return *this;
    }

    ButtonConfigBuilder& square(bool square)
    {
        m_config.square = square;
        return *this;
    }

    ui::styling::ButtonConfig&& build()
    {
        return std::move(m_config);
    }

private:
    ui::styling::ButtonConfig m_config;
};