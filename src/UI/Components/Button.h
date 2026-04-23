#pragma once
#include <string>
#include <functional>

namespace ui::components
{

    class Button
    {
    public:
        Button(const std::string& name, std::function<void()> action)
            : m_name(name), m_action(action)
        { }

        virtual ~Button() = default;

        const std::string& name() const
        {
            return m_name;
        }

        void execute()
        {
            m_action();
        }

    private:
        std::string           m_name;
        std::function<void()> m_action;
    };

} // ui::components