#include "RadioImageButton.h"
#include "../Styling/Button.h"

namespace ui::components
{

    RadioImageButton::RadioImageButton(
        const std::string&                    name,
        const std::string&                    iconFilePath,
        std::vector<RadioButton*>&            group,
        std::shared_ptr<ui::styling::IConfig> config,
        std::function<void(Button*)>          action)
        : Button(name, std::move(config), std::move(action))
        , ImageButton(name, iconFilePath, std::move(config), std::move(action))
        , RadioButton(name, group, std::move(config), std::move(action))
    { }

    void RadioImageButton::render()
    {
        ui::styling::Button::render<ui::ButtonType::RADIO_IMAGE>(this);
    }

} // ui::components