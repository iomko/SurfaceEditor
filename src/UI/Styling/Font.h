#pragma once
#include <imgui.h>

namespace ui::styling
{

    class Font
    {
    public:
        Font() = delete;

        static ImFont* regular(float size)
        {
            auto& io = ImGui::GetIO();
            return io.Fonts->AddFontFromFileTTF("../fonts/OpenSans-Regular.ttf", size);
        }

        static ImFont* bold(float size)
        {
            auto& io = ImGui::GetIO();
            return io.Fonts->AddFontFromFileTTF("../fonts/OpenSans_Condensed-Bold.ttf", size);
        }

        static ImFont* extraBold(float size)
        {
            auto& io = ImGui::GetIO();
            return io.Fonts->AddFontFromFileTTF("../fonts/OpenSans_Condensed-ExtraBold.ttf", size);
        }
    };

} // ui::styling