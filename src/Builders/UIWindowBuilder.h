#pragma once
#include "../UI/Styling/Window.h"

class WindowPosConfigBuilder
{
public:
    WindowPosConfigBuilder()
        : m_config{}
    { }

    WindowPosConfigBuilder& posX(float x)
    {
        m_config.posX = x;
        return *this;
    }

    WindowPosConfigBuilder& posY(float y)
    {
        m_config.posY = y;
        return *this;
    }

    ui::styling::WindowPosConfig&& build()
    {
        return std::move(m_config);   
    }

private:
    ui::styling::WindowPosConfig m_config;
};

class WindowSizeConfigBuilder
{
public:
    WindowSizeConfigBuilder()
        : m_config{}
    { }

    WindowSizeConfigBuilder& width(float windowWidth)
    {
        m_config.width = windowWidth;
        return *this;
    }

    WindowSizeConfigBuilder& height(float windowHeight)
    {
        m_config.height = windowHeight;
        return *this;
    }

    WindowSizeConfigBuilder& minWidth(float windowMinWidth)
    {
        m_config.minWidth = windowMinWidth;
        return *this;
    }

    WindowSizeConfigBuilder& minHeight(float windowMinHeight)
    {
        m_config.minHeight = windowMinHeight;
        return *this;
    }

    ui::styling::WindowSizeConfig&& build()
    {
        return std::move(m_config);
    }

private:
    ui::styling::WindowSizeConfig m_config;
};

class WindowConfigBuilder
{
public:
    WindowConfigBuilder()
        : m_config{}
    { }

    WindowConfigBuilder& name(const std::string& name)
    {
        m_config.name = name;
        return *this;
    }

    WindowConfigBuilder& background(const ImVec4& background)
    {
        m_config.backgroundColor = background;
        return *this;
    }

    WindowConfigBuilder& rounding(float rounding)
    {
        m_config.rounding = rounding;
        return *this;
    }

    WindowConfigBuilder& flags(ImGuiWindowFlags flags)
    {
        m_config.flags = flags;
        return *this;
    }

    WindowConfigBuilder& pos(const ui::styling::WindowPosConfig& pos)
    {
        m_config.pos = pos;
        return *this;
    }

    WindowConfigBuilder& size(const ui::styling::WindowSizeConfig& size)
    {
        m_config.size = size;
        return *this;
    }

    ui::styling::WindowConfig&& build()
    {
        return std::move(m_config);
    }

private:
    ui::styling::WindowConfig m_config;
};
