#pragma once
#include "../UI/Styling/InputBox.h"

class InputBoxBuilder
{
public:
    InputBoxBuilder()
        : m_config{}
    { }

    InputBoxBuilder& width(float width)
    {
        m_config.width = width;
        return *this;
    }

    InputBoxBuilder& background(const ImVec4& background)
    {
        m_config.background = background;
        return *this;
    }

    ui::styling::InputBoxConfig&& build()
    {
        return std::move(m_config);
    }

private:
    ui::styling::InputBoxConfig m_config;
};