#pragma once
#include <imgui.h>
#include "RadioButton.h"
#include "../../Renderer/ImageLoader.h"

class ImageButton : public RadioButton
{
public:
    ImageButton(
        const std::string name,
        const std::string iconFilePath,
        std::function<void(Button*)> callback,
        std::function<void()> endCallback
    );

    ~ImageButton() = default;

    inline const int textureID() const { return m_textureID; }

private:
    void loadIcon();

private:
    const std::string m_iconFilePath;
    ImTextureID m_textureID;
};