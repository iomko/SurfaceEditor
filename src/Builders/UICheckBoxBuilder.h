#pragma once
#include <utility>
#include "../UI/Styling/CheckBox.h"

class CheckBoxConfigBuilder
{
public:
    CheckBoxConfigBuilder()
        : m_config{}
    { }

    CheckBoxConfigBuilder& background(const ImVec4& background)
    {
        m_config.background = background;
        return *this;
    }

    CheckBoxConfigBuilder& onHoverBackground(const ImVec4& background)
    {
        m_config.hoveredBackground = background;
        return *this;
    }

    CheckBoxConfigBuilder& onActiveBackground(const ImVec4& background)
    {
        m_config.activeBackground = background;
        return *this;
    }

    CheckBoxConfigBuilder& checkMarkColor(const ImVec4& color)
    {
        m_config.checkMarkColor = color;
        return *this;
    }

    CheckBoxConfigBuilder& framePadding(const ImVec2& padding)
    {
        m_config.framePadding = padding;
        return *this;
    }

    std::shared_ptr<ui::styling::CheckBoxConfig> build()
    {
        return std::make_shared<ui::styling::CheckBoxConfig>(std::move(m_config));
    }

private:
    ui::styling::CheckBoxConfig m_config;
};