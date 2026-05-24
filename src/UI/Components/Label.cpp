#include "Label.h"
#include "../Styling/Label.h"

namespace ui::components
{

    Label::Label(const std::string& text, std::shared_ptr<ui::styling::IConfig> config)
        : IComponent(text, std::move(config))
    { }

    void Label::render()
    {
        ui::styling::Label::render(this);
    }

} // ui::components