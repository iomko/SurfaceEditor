#pragma once
#include <imgui.h>
#include <string>
#include <functional>

namespace ui
{
    struct AppliedStyling
    {
        int appliedColorStyles{};
        int appliedVarStyles{};
    };

    struct WindowPosConfig
    {
        float       posX;         // represents percentage of screen to achieve responsive design
        float       posY;         // represents percentage of screen to achieve responsive design
        ImVec2      rawPos;       // used automaticly by OverlappingWindow class
    };

    struct WindowSizeConfig
    {
        float  width{};     // represents percentage of screen to achieve responsive design - implicitly responsive (no need to define)
        float  height{};    // represents percentage of screen to achieve responsive design - implicitly responsive (no need to define)
        float  minWidth;    // represents percentage of screen to achieve responsive design
        float  minHeight;   // represents percentage of screen to achieve responsive design
        ImVec2 rawSize;     // used automaticly by OverlappingWindow class
    };

    struct WindowConfig
    {
        const char*      name;
        std::string      layerName;
        ImVec4           backgroundColor;
        float            rounding;
        ImGuiWindowFlags flags;
        WindowPosConfig  pos;
        WindowSizeConfig size;
        AppliedStyling   styles;
    };

    class Window
    {
    public:
        Window() = delete;

        ~Window() = delete;

        static void setPosAndSize(const std::string& layerName, WindowPosConfig& posConfig, WindowSizeConfig& sizeConfig);

        static void init(WindowConfig& config);

        static void destroy(WindowConfig& config);

        static void addToLayout(std::function<void()> asignComponents);
    };
} // ui