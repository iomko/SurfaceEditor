#pragma once
#include "../Styling/Slider.h"

namespace ui::components
{

    template<typename T>
    class Slider
    {
    public:
        Slider(const std::string& name, const ui::styling::SliderConfig<T>& config)
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

        ui::styling::SliderConfig<T>& config()
        {
            return m_config;
        }

    private:
        T                            m_inputValue;
        std::string                  m_name;
        ui::styling::SliderConfig<T> m_config;
    };

} // ui::components