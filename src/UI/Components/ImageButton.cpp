#include "ImageButton.h"

ImageButton::ImageButton(
    const std::string name,
    const std::string iconFilePath,
    std::function<void(Button*)> callback,
    std::function<void()> endCallback
) : RadioButton(name, callback, endCallback), m_iconFilePath{iconFilePath}
{
    loadIcon();
}

void ImageButton::loadIcon()
{
    m_textureID = (ImTextureID)(intptr_t)ImageLoader::loadImage(m_iconFilePath.c_str());
}