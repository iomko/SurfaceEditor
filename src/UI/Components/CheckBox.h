#pragma once
#include <string>
#include "../Styling/CheckBox.h"

namespace ui::components
{

    class CheckBox
    {
    public:
        CheckBox(const std::string& name, const ui::styling::CheckBoxConfig& config)
            : m_name(name)
            , m_isChecked{}
            , m_config(config)
        { }

        bool* isChecked()
        {
            return &m_isChecked;
        }

        const std::string& name() const
        {
            return m_name;
        }

        ui::styling::CheckBoxConfig& config()
        {
            return m_config;
        }

    private:
        bool                        m_isChecked;
        std::string                 m_name;
        ui::styling::CheckBoxConfig m_config;
    };

} // ui::components