#pragma once
#include <string>
#include "../Styling/Common.h"
#include "../Components/Common.h"

namespace ui::components
{

    class CheckBox : public IComponent
    {
    public:
        CheckBox(const std::string& name, std::shared_ptr<ui::styling::IConfig> config)
            : IComponent(name, std::move(config))
            , m_isChecked{}
        { }

        void render() override
        {
            ui::styling::CheckBox::render(this);
        }

        bool* isChecked()
        {
            return &m_isChecked;
        }

    private:
        bool m_isChecked;
    };

} // ui::components