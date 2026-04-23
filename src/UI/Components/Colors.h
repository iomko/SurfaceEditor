#pragma once
#include <imgui.h>

namespace ui
{
    
    struct Color
    {
        static constexpr ImVec4 gray            = ImVec4(0.15f, 0.15f, 0.17f, 0.85f);
        static constexpr ImVec4 transparentGray = ImVec4(0.15f, 0.15f, 0.17f, 0.70f);
    };

} // ui