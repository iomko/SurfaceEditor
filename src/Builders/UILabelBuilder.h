#pragma once
#include <stdexcept>
#include "../UI/Styling/Label.h"

class LabelConfigBuilder
{
public:
    LabelConfigBuilder()
        : m_config{}
    {
        m_config.color = ImVec4{ 1.0f, 1.0f, 1.0f, 1.0f };
    }

    LabelConfigBuilder& font(ImFont* font)
    {
        m_config.font = font;
        return *this;
    }

    LabelConfigBuilder& color(const ImVec4& color)
    {
        m_config.color = color;
        return *this;
    }

    ui::styling::LabelConfig&& build()
    {
        if (m_config.font == nullptr)
        {
            throw std::logic_error("Label construcion error: font parameter is mandatory");
        }

        return std::move(m_config);
    }

private:
    ui::styling::LabelConfig m_config;
};