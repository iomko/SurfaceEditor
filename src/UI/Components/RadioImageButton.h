#pragma once
#include "ImageButton.h"
#include "RadioButton.h"

namespace ui::components
{

    class RadioImageButton : public ImageButton, public RadioButton
    {
    public:
        RadioImageButton(const std::string& name,
                         const std::string& iconFilePath,
                         std::shared_ptr<ui::styling::IConfig> config,
                         std::function<void(Button*)> action)
            : Button(name, std::move(config), std::move(action))
            , ImageButton(name, iconFilePath, std::move(config), std::move(action))
            , RadioButton(name, std::move(config), std::move(action))
        { }

        void render() override
        {
            ui::styling::Button::render(this);
        }
    };

} // ui::components