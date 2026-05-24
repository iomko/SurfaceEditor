#include "CheckBox.h"
#include "../Styling/CheckBox.h"

namespace ui::components
{

    CheckBox::CheckBox(const std::string& name, std::shared_ptr<ui::styling::IConfig> config)
        : IComponent(name, std::move(config))
        , m_isChecked{}
    { }

    void CheckBox::render()
    {
        ui::styling::CheckBox::render(this);
    }

    bool* CheckBox::isChecked()
    {
        return &m_isChecked;
    }

} // ui::components
