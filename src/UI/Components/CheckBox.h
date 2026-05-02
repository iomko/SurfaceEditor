#pragma once
#include "../Styling/CheckBox.h"

namespace ui::components
{

    class CheckBox
    {
    public:
        CheckBox(const ui::styling::CheckBoxConfig& config)
            : m_isChecked{}
            , m_config(config)
        { }

        bool* isChecked()
        {
            return &m_isChecked;
        }

        ui::styling::CheckBoxConfig& config()
        {
            return m_config;
        }

    private:
        bool                        m_isChecked;
        ui::styling::CheckBoxConfig m_config;
    };

} // ui::components