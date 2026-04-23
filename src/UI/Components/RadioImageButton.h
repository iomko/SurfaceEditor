#pragma once
#include "ImageButton.h"
#include "RadioButton.h"

namespace ui::components
{

    class RadioImageButton : public ImageButton, public RadioButton
    {
    public:
        RadioImageButton(const std::string& name, const std::string& iconFilePath, std::function<void()> action)
            : Button(name, action), ImageButton(name, iconFilePath, action), RadioButton(name, action)
        { }
    };

} // ui::components