#pragma once
#include <string>
#include <imgui.h>
#include "../../Renderer/ImageLoader.h"

class ImageButton
{
public:
    ImageButton(const std::string& name, const std::string& iconFilePath)
        : m_name(name), m_textureID((ImTextureID)(intptr_t)ImageLoader::loadImage(iconFilePath.c_str()))
    { }

    const std::string& name() const
    {
        return m_name;
    }

    const int textureID() const
    {
        return m_textureID;
    }

private:
    std::string m_name;
    ImTextureID m_textureID;
};