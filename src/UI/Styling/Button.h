#pragma once
#include "Common.h"
#include "../Components/Common.h"

namespace ui::styling
{

    struct ButtonConfig : IConfig
    {
        bool    square;
        float   rounding;
        ImFont* font;
        ImVec2  size;
        ImVec2  realSize;
        ImVec4  background;
        ImVec4  onHoverOverColor;
        ImVec4  onClickColor;
        ImVec2  framePadding;
    };

    class Button
    {
    public:
        Button() = delete;

        static void render(ui::components::Button* button);
    };

} // ui::styling