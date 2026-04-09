#include "FontStyle.h"

void FontStyle::init(ImGuiIO& io)
{
    io.Fonts->AddFontDefault();

    s_regular   = io.Fonts->AddFontFromFileTTF("../fonts/OpenSans-Regular.ttf", 16.0f);
    s_bold      = io.Fonts->AddFontFromFileTTF("../fonts/OpenSans_Condensed-Bold.ttf", 20.0f);
    s_extraBold = io.Fonts->AddFontFromFileTTF("../fonts/OpenSans_Condensed-ExtraBold.ttf", 20.0f);
}

void FontStyle::headliner()
{
    ImGui::PushFont(s_extraBold);
}

void FontStyle::regular()
{
    ImGui::PushFont(s_regular);
}

void FontStyle::end()
{
    ImGui::PopFont();
}