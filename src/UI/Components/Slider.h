#pragma once
#include "../Styling/Slider.h"

namespace ui::components
{

    template<typename T>
    class Slider : public IComponent
    {
    public:
        Slider(const std::string& name, std::shared_ptr<ui::styling::IConfig> config)
            : IComponent(name, std::move(config))
            , m_inputValue{}
        { }

        const std::string name() const override
        {
            return "##" + m_name;
        }

        void render() override
        {
            ui::styling::Slider::render(this);
        }

        T* inputValue()
        {
            return &m_inputValue;
        }

    private:
        T m_inputValue;
    };

} // ui::components