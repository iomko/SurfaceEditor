#pragma once
#include <imgui.h>
#include "Button.h"
#include "../../Renderer/ImageLoader.h"

namespace ui::components
{

    class ImageButton : public virtual Button
    {
    public:
        ImageButton(const std::string& name,
                    const std::string& iconFilePath,
                    std::shared_ptr<ui::styling::IConfig> config,
                    std::function<void(Button*)> action)
            : Button(name, std::move(config), std::move(action))
            , m_textureID((ImTextureID)(intptr_t)ImageLoader::loadImage(iconFilePath.c_str()))
        { }

        virtual ~ImageButton() = default;

        void render() override
        {
            ui::styling::Button::render(this);
        }

        const int textureID() const
        {
            return m_textureID;
        }

    private:
        ImTextureID m_textureID;
    };

} // ui::components