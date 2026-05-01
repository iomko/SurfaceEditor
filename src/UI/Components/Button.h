#pragma once
#include <string>
#include <functional>
#include "../Styling/Button.h"

namespace ui::components
{

    class Button
    {
    public:
        Button(const std::string& name, const ui::styling::ButtonConfig& config, std::function<void(Button*)> action)
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
            m_action(this);
        }

    private:
        std::string                  m_name;
        ui::styling::ButtonConfig    m_config;
        std::function<void(Button*)> m_action;
    };

} // ui::components