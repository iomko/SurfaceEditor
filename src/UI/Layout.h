#pragma once
#include <vector>
#include "Components/Common.h"

namespace ui
{

    class Layout
    {
    public:
        Layout(const ImVec2& margin = {})
            : m_margin(margin)
            , m_isVisible(true)
        { }

        const bool isVisible() const
        {
            return m_isVisible;
        }

        const ImVec2& margin() const
        {
            return m_margin;
        }

        const std::vector<std::unique_ptr<components::IComponent>>& components() const
        {
            return m_components;
        }

        void reserveComponents(int count)
        {
            m_components.reserve(count);
        }

        void asignComponent(std::unique_ptr<components::IComponent> component)
        {
            m_components.emplace_back(std::move(component));
        }

        void setIsVisible(bool isVisible)
        {
            m_isVisible = isVisible;
        }

    private:
        bool                                                 m_isVisible;
        const ImVec2                                         m_margin;
        std::vector<std::unique_ptr<components::IComponent>> m_components;
    };

} // ui