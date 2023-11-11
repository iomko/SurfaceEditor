#pragma once
#include "Renderer.h"
#include "Window.h"

class Application
{
public:
    static Application& getInstance()
    {
        static Application instance;
        return instance;
    }

    // Access the window instance
    Window& getWindow()
    {
        return window;
    }

    void close()
    {
        window.terminate();
    }

    void run()
    {
        window.setCallBackFunctions();
        glfwSetInputMode(window.windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

private:
    Application() : window(800, 600, "SurfaceEditor"){}
    Window window;
    Renderer renderer;
};