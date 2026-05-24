#pragma once
#include <imgui.h>
#include "Button.h"
#include "../../Renderer/ImageLoader.h"

namespace ui::components
{

    class ImageButton : public virtual Button
    {
    public:
        ImageButton(
            const std::string&                    name,
            const std::string&                    iconFilePath,
            std::shared_ptr<ui::styling::IConfig> config,
            std::function<void(Button*)>          action);

        virtual ~ImageButton() = default;

        void render() override;

        const int textureID() const;

    private:
        ImTextureID m_textureID;
    };

} // ui::components