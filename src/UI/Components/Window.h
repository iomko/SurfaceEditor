#pragma once
#include <imgui.h>
#include <string>
#include <functional>

namespace ui
{
    enum class Orientation
    {
        HORIZONTAL,
        VERTICAL
    };

    struct AppliedStyling
    {
        int appliedColorStyles;
        int appliedVarStyles;
    };

    struct WindowPosConfig
    {
        std::string layer;
        float       posX;         // represents percentage of screen to achieve responsive design
        float       posY;         // represents percentage of screen to achieve responsive design
        float       width;        // represents percentage of screen to achieve responsive design
        float       height;       // represents percentage of screen to achieve responsive design
        float       minWidth;     // represents percentage of screen to achieve responsive design
        float       minHeight;    // represents percentage of screen to achieve responsive design
        ImVec2      rawPos;       // used automaticly by OverlappingWindow class
        ImVec2      rawSize;      // used automaticly by OverlappingWindow class
    };

    struct WindowConfig
    {
        const char*      name;
        bool             transparent;
        float            rounding;
        ImGuiWindowFlags flags;
        AppliedStyling   styles;
    };

    class Window
    {
    public:
        Window() = delete;

        ~Window() = delete;

        static void setPosAndSize(WindowPosConfig& config);

        static void init(WindowConfig& config);

        static void destroy(WindowConfig& config);

        static void addToLayout(float margin, std::function<void()> asignComponents);

        static void nextItem(const Orientation& orientation);

        static void drawSeparator(const Orientation& orientation, float length);
    };
} // ui