#pragma once
#include "../Components/Common.h"
#include "../Styling/InputBox.h"

namespace ui::components
{
    
    template<typename T>
    class InputBox : public IComponent
    {
    public:
        InputBox(const std::string& name, std::shared_ptr<ui::styling::IConfig> config)
            : IComponent(name, std::move(config))
            , m_inputValue{}
        { }

        void render() override
        {
            ui::styling::InputBox::render<T>(m_name, &m_inputValue, m_config.get());
        }

        T* inputValue()
        {
            return &m_inputValue;
        }

    private:
        T m_inputValue;
    };

    template<>
    class InputBox<char> : public IComponent
    {
    public:
        InputBox(const std::string& name, std::shared_ptr<ui::styling::IConfig> config)
            : IComponent(name, std::move(config))
        { }

        void render() override
        {
            ui::styling::InputBox::render<char>(m_name, m_inputValue, m_config.get());
        }

        char* inputValue()
        {
            return m_inputValue;
        }

    private:
        static constexpr int BUFFER_SIZE = 256;

        char m_inputValue[BUFFER_SIZE];
    };

} // ui::components
