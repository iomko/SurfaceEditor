#pragma once
#include <string>
#include <imgui.h>
#include "Common.h"

namespace ui::styling
{

    struct CheckBoxConfig
    {
        ImVec4         checkMarkColor;
        ImVec4         background;
        ImVec4         hoveredBackground;
        ImVec4         activeBackground;
        ImVec2         framePadding;
        AppliedStyling styles;
    };

    class CheckBox
    {
    public:
        CheckBox() = delete;

        ~CheckBox() = delete;

        static void init(const std::string& name, bool* isChecked, CheckBoxConfig& config);

        static void destroy(CheckBoxConfig& config);
    };

} // ui::styling