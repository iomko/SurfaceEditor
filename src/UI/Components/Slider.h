#pragma once
#include "../Styling/Slider.h"

namespace ui::components
{

    template<typename T>
    class Slider : public IComponent
    {
    public:
        Slider(const std::string& name, std::shared_ptr<ui::styling::IConfig> config)
            : IComponent("##" + name, std::move(config))
            , m_inputValue{}
        { }

        void render() override
        {
            ui::styling::Slider::render(m_name, &m_inputValue, m_config.get());
        }

        T* inputValue()
        {
            return &m_inputValue;
        }

    private:
        T m_inputValue;
    };

} // ui::components