#pragma once
#include "Button.h"

namespace ui::components
{

    class RadioButton : public virtual Button
    {
    public:
        RadioButton(const std::string& name, std::shared_ptr<ui::styling::IConfig> config, std::function<void(Button*)> action)
            : Button(name, std::move(config), std::move(action))
        { }

        virtual ~RadioButton() = default;

        void render() override
        {
            ui::styling::Button::render(this);
        }

        const bool& isSelected() const
        {
            return m_isSelected;
        }

        void setIsSelected(const bool& isSelected)
        {
            m_isSelected = isSelected;
        }

    private:
        bool m_isSelected;
    };

} // ui::components