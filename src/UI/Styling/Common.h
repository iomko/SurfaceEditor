#pragma once
#include <imgui.h>

namespace ui::styling
{

    struct AppliedStyling
    {
        int appliedColorStyles{};
        int appliedVarStyles{};
    };
    
    struct Color
    {
        static constexpr ImVec4 black           = ImVec4(0, 0, 0, 0);
        static constexpr ImVec4 gray            = ImVec4(0.15f, 0.15f, 0.17f, 0.85f);
        static constexpr ImVec4 darkGray        = ImVec4(0.1f, 0.1f, 0.1f, 0.8f);
        static constexpr ImVec4 lightGray       = ImVec4(0.25f, 0.25f, 0.25f, 1.0f);
        static constexpr ImVec4 transparentGray = ImVec4(0.15f, 0.15f, 0.17f, 0.70f);
        static constexpr ImVec4 titleBarOrange  = ImVec4(1.00f, 0.45f, 0.00f, 1.0f);
        static constexpr ImVec4 hoverOverOrange = ImVec4(1.0f, 0.55f, 0.0f, 0.35f);
        static constexpr ImVec4 onClickOrange   = ImVec4(1.0f, 0.55f, 0.0f, 0.55f);
        static constexpr ImVec4 selectedOrange  = ImVec4(1.0f, 0.55f, 0.0f, 0.35f);
    };

    struct IConfig
    {
        virtual ~IConfig() = default;
        AppliedStyling styles;
    };

} // ui
