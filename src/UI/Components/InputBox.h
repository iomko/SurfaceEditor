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

    template<>
    class InputBox<char> : public IComponent
    {
    public:
        InputBox(const std::string& name, std::shared_ptr<ui::styling::IConfig> config)
            : IComponent("##" + name, std::move(config))
            , m_buffer{}
        { }

        void render() override
        {
            ui::styling::InputBox::render<char>(m_name, m_buffer, m_config.get());
        }

        char* inputValue()
        {
            return m_buffer;
        }

    private:
        static constexpr int BUFFER_SIZE = 256;
        
        char m_buffer[BUFFER_SIZE];
    };

} // ui::components
