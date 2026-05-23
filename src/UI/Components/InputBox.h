#pragma once
#include "../Styling/InputBox.h"
#include "../Styling/Common.h"
#include "../Components/Common.h"

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
            ui::styling::InputBox::render<T>(this);
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
            ui::styling::InputBox::render<char>(this);
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