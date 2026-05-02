#pragma once
#include "../Styling/InputBox.h"

namespace ui::components
{

    template<typename T>
    class InputBox
    {
    public:
        InputBox(const std::string& name, const ui::styling::InputBoxConfig& config)
            : m_name(name)
            , m_config(config)
            , m_inputValue{}
        { }

        const std::string name() const
        {
            return "##" + m_name;
        }

        T* inputValue()
        {
            return &m_inputValue;
        }

        ui::styling::InputBoxConfig& config()
        {
            return m_config;
        }

    private:
        T                           m_inputValue;
        std::string                 m_name;
        ui::styling::InputBoxConfig m_config;
    };

    template<>
    class InputBox<char>
    {
    public:
        InputBox(const std::string& name, const ui::styling::InputBoxConfig& config)
            : m_name(name)
            , m_config(config)
        { }

        const std::string& name() const
        {
            return m_name;
        }

        char* inputValue()
        {
            return m_inputValue;
        }

        ui::styling::InputBoxConfig& config()
        {
            return m_config;
        }

    private:
        static constexpr int BUFFER_SIZE = 256;

        ui::styling::InputBoxConfig m_config;
        std::string                 m_name;
        char                        m_inputValue[BUFFER_SIZE];
    };

} // ui::components