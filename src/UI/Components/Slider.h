#pragma once
#include "../Components/Linkable.h"
#include "../Styling/Slider.h"

namespace ui::components
{

    template<typename T>
    class Slider : public IComponent, public Linkable<T>
    {
    public:
        Slider(const std::string& name, std::shared_ptr<ui::styling::IConfig> config)
            : IComponent("##" + name, std::move(config))
        { }

        void render() override
        {
            ui::styling::Slider::render(m_name, this->m_valPtr, m_config.get());
        }

        T* inputValue() const
        {
            return this->m_valPtr;
        }
    };

} // ui::components