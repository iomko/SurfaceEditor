#include "Button.h"
#include "../Styling/Button.h"

namespace ui::components
{

    Button::Button(const std::string& name, std::shared_ptr<ui::styling::IConfig> config, std::function<void(Button*)> action)
        : IComponent(name, std::move(config))
        , m_action(std::move(action))
    { }

    void Button::render()
    {
        ui::styling::Button::render(this);
    }

    void Button::setAction(std::function<void(Button*)> action)
    {
        m_action = std::move(action);
    }

    void Button::execute()
    {
        m_action(this);
    }

} // ui::components