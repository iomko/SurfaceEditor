#include "ImageButton.h"

ImageButton::ImageButton(
    const std::string iconFilePath, const ImVec2 size,
    std::function<void(ImageButton*)> callback, std::function<void()> endCallback
) :
    m_isSelected{}, m_iconFilePath{iconFilePath},
    m_size{size}, m_callback{callback}, m_endCallback{endCallback}
{
    loadIcon();
}

void ImageButton::loadIcon()
{
    m_textureID = (ImTextureID)(intptr_t)ImageLoader::loadImage(m_iconFilePath.c_str());
}