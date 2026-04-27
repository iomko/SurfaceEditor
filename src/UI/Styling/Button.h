#pragma once
#include "Common.h"

namespace ui::styling
{

    struct ButtonConfig
    {
        float          rounding;
        ImVec4         background;
        ImVec4         onHoverOverColor;
        ImVec4         onClickColor;
        ImVec2         framePadding;
        ImVec2         windowPadding;
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