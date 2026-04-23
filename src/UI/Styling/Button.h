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
        AppliedStyling styles;
    };

    class Button
    {
    public:
        Button() = delete;

        ~Button() = delete;

        static void init(ButtonConfig& config);

        static void destroy(ButtonConfig& config);

        static void changeBackgroundColor(ButtonConfig& config);
    };

} // ui::styling