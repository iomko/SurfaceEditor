#pragma once
#include <imgui.h>

namespace ui::styling
{

    struct AppliedStyling
    {
        int appliedColorStyles;
        int appliedVarStyles;
    };
    
    struct Color
    {
        static constexpr ImVec4 black           = ImVec4(0, 0, 0, 0);
        static constexpr ImVec4 darkGray        = ImVec4(0.1f, 0.1f, 0.1f, 0.8f);
        static constexpr ImVec4 gray            = ImVec4(0.15f, 0.15f, 0.17f, 0.85f);
        static constexpr ImVec4 transparentGray = ImVec4(0.15f, 0.15f, 0.17f, 0.70f);
        static constexpr ImVec4 hoverOverOrange = ImVec4(1.0f, 0.55f, 0.0f, 0.35f);
        static constexpr ImVec4 onClickOrange   = ImVec4(1.0f, 0.55f, 0.0f, 0.55f);
        static constexpr ImVec4 selectedOrange  = ImVec4(1.0f, 0.55f, 0.0f, 0.35f);
    };

} // ui