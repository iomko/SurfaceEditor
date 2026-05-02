#pragma once
#include <string>
#include "../Styling/Label.h"

namespace ui::components
{

    class Label
    {
    public:
        Label(const std::string& text, const ui::styling::LabelConfig& config)
            : m_text(text)
            , m_config(config)
        { }

        const std::string& text() const
        {
            return m_text;
        }

        ui::styling::LabelConfig& config()
        {
            return m_config;
        }

    private:
        std::string              m_text;
        ui::styling::LabelConfig m_config;
    };

} // ui::components