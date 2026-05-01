#pragma once
#include "ImageButton.h"
#include "RadioButton.h"

namespace ui::components
{

    class RadioImageButton : public ImageButton, public RadioButton
    {
    public:
        RadioImageButton(const std::string& name,
                         const ui::styling::ButtonConfig& config,
                         const std::string& iconFilePath,
                         std::function<void(Button*)> action)
            : Button(name, config, action),
              ImageButton(name, config, iconFilePath, action),
              RadioButton(name, config, action)
        { }
    };

} // ui::components