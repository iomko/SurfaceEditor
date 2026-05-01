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
                    const ui::styling::ButtonConfig& config,
                    const std::string& iconFilePath,
                    std::function<void(Button*)> action)
            : Button(name, config, action),
              m_textureID((ImTextureID)(intptr_t)ImageLoader::loadImage(iconFilePath.c_str()))
        { }

        virtual ~ImageButton() = default;

        const int textureID() const
        {
            return m_textureID;
        }

    private:
        ImTextureID m_textureID;
    };

} // ui::components