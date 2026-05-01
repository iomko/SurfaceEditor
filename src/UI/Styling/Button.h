#pragma once
#include "Common.h"

namespace ui::styling
{

    struct ButtonConfig
    {
        bool           square;
        float          rounding;
        ImFont*        font;
        ImVec2         size;
        ImVec2         realSize;
        ImVec4         background;
        ImVec4         onHoverOverColor;
        ImVec4         onClickColor;
        ImVec2         framePadding;
        AppliedStyling styles;
    };

    class Button
    {
    public:
        Button() = delete;

        ~Button() = delete;

        static void init(ButtonConfig& config);

        static void destroy(ButtonConfig& config);
    };

} // ui::styling