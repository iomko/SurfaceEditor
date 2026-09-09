#pragma once
#include <Styling/InputBox.h>

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

    std::shared_ptr<ui::styling::InputBoxConfig> build()
    {
        return std::make_shared<ui::styling::InputBoxConfig>(std::move(m_config));
    }

private:
    ui::styling::InputBoxConfig m_config;
};