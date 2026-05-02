#pragma once
#include <string>
#include <imgui.h>

namespace ui::styling
{

    struct LabelConfig
    {
        ImFont* font;
        ImVec4  color;
    };

    class Label
    {
    public:
        Label() = delete;

        ~Label() = delete;

        static void init(const std::string& text, LabelConfig& config);

        static void destroy(LabelConfig& config);
    };

} // ui::styling