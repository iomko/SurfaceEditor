#pragma once
#include <functional>
#include <string>
#include <imgui.h>
#include "../../Renderer/ImageLoader.h"

class ImageButton
{
public:
    ImageButton(
        const std::string iconFilePath, const ImVec2 size,
        std::function<void(ImageButton*)> callback, std::function<void()> endCallback
    );

    ~ImageButton() = default;

    inline const int textureID() const { return m_textureID; }

    inline const std::string& name() const { return m_iconFilePath; }

    inline bool& isSelected() { return m_isSelected; }

    inline void execute() { m_callback(this); }

    inline void end() { m_endCallback(); };

private:
    void loadIcon();

private:
    bool m_isSelected;
    const ImVec2 m_size;
    const std::string m_iconFilePath;
    ImTextureID m_textureID;
    std::function<void(ImageButton*)> m_callback;
    std::function<void()> m_endCallback;
};