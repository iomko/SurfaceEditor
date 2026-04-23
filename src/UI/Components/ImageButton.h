#pragma once
#include <imgui.h>
#include "Button.h"
#include "../../Renderer/ImageLoader.h"

namespace ui::components
{

    class ImageButton : public virtual Button
    {
    public:
        ImageButton(const std::string& name, const std::string& iconFilePath, std::function<void()> action)
            : Button(name, action), m_textureID((ImTextureID)(intptr_t)ImageLoader::loadImage(iconFilePath.c_str()))
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