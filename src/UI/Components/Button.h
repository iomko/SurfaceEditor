#pragma once
#include <string>
#include <functional>
#include "../Styling/Button.h"

namespace ui::components
{

    class Button
    {
    public:
        Button(const std::string& name, const ui::styling::ButtonConfig& config, std::function<void()> action)
            : m_name(name),
              m_config(config),
              m_action(action)
        { }

        virtual ~Button() = default;

        const std::string& name() const
        {
            return m_name;
        }

        ui::styling::ButtonConfig& config()
        {
            return m_config;
        }

        void execute()
        {
            m_action();
        }

    private:
        std::string               m_name;
        std::function<void()>     m_action;
        ui::styling::ButtonConfig m_config;
    };

} // ui::components