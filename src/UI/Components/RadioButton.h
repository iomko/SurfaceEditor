#pragma once
#include "Button.h"

namespace ui::components
{

    class RadioButton : public virtual Button
    {
    public:
        RadioButton(const std::string& name, const ui::styling::ButtonConfig& config, std::function<void(Button*)> action)
            : Button(name, config, action)
        { }

        virtual ~RadioButton() = default;

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