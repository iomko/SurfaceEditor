#pragma once
#include <imgui.h>
#include "Common.h"

namespace ui::components
{

    class Dummy : public IComponent
    {
    public:
        Dummy(float x, float y)
            : IComponent({}, {})
            , m_x(x)
            , m_y(y)
        { }

        void render() override
        {
            ImGui::Dummy(ImVec2{ImGui::GetWindowSize().x * m_x, ImGui::GetWindowSize().y * m_y});
        }

    private:
        float m_x;
        float m_y;
    };

    class RelativeDummy : public IComponent
    {
    public:
        RelativeDummy(float x, float y)
            : IComponent({}, {})
            , m_x(x)
            , m_y(y)
        { }

        void render() override
        {
            ImGui::Dummy(ImVec2{m_x, m_y});
        }

    private:
        float m_x;
        float m_y;
    };

} // ui::components
