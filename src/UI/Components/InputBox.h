#pragma once
#include "../Components/Common.h"
#include "../Components/Linkable.h"
#include "../Styling/InputBox.h"

namespace ui::components
{
    
    template<typename T>
    class InputBox : public IComponent, public Linkable<T>
    {
    public:
        InputBox(const std::string& name, std::shared_ptr<ui::styling::IConfig> config)
            : IComponent("##" + name, std::move(config))
        { }

        void render() override
        {
            ui::styling::InputBox::render<T>(m_name, this->m_valPtr, m_config.get());
        }

        T* inputValue() const
        {
            return this->m_valPtr;
        }
    };

} // ui::components
