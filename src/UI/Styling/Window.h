#pragma once
#include <imgui.h>
#include <string>
#include <functional>
#include "Common.h"

namespace ui::styling
{

    struct WindowPosConfig
    {
        bool   relativePosition;
        float  posX;              // represents percentage of screen to achieve responsive design
        float  posY;              // represents percentage of screen to achieve responsive design
        ImVec2 rawPos;            // used automaticly by IWindow class
    };

    struct WindowSizeConfig
    {
        float  width;       // represents percentage of screen to achieve responsive design - implicitly responsive (no need to define)
        float  height;      // represents percentage of screen to achieve responsive design - implicitly responsive (no need to define)
        float  minWidth;    // represents percentage of screen to achieve responsive design
        float  minHeight;   // represents percentage of screen to achieve responsive design
        ImVec2 rawSize;     // used automaticly by IWindow class
        ImVec2 realSize;    // auto fit size value
    };

    struct WindowTitleBarConfig
    {
        ImVec4  background;
        ImFont* font;
    };

    struct WindowConfig
    {
        std::string          name;
        std::string          layerName;
        ImVec4               backgroundColor;
        float                rounding;
        ImGuiWindowFlags     flags;
        WindowPosConfig      pos;
        WindowSizeConfig     size;
        WindowTitleBarConfig titleBar;
        AppliedStyling       styles;
    };

    class Window
    {
    public:
        Window() = delete;

        static void setPosAndSize(WindowConfig& config);

        static void setRelativePosAndSize(WindowConfig& config);

        static void init(WindowConfig& config);

        static void destroy(WindowConfig& config);

        static void addToLayout(std::function<void()> asignComponents, const ImVec2& margin = {});
    };
} // ui::styling