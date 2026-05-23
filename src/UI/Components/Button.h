#pragma once
#include <string>
#include <functional>
#include "../Components/Common.h"

namespace ui::components
{

    class Button : public IComponent
    {
    public:
        Button(const std::string& name, std::shared_ptr<ui::styling::IConfig> config, std::function<void(Button*)> action)
            : IComponent(name, std::move(config))
            , m_action(std::move(action))
        { }

        virtual ~Button() = default;

        void render() override
        {
            ui::styling::Button::render(this);
        }

        void setAction(std::function<void(Button*)> action)
        {
            m_action = std::move(action);
        }

        void execute()
        {
            m_action(this);
        }

    private:
        std::function<void(Button*)> m_action;
    };

} // ui::components