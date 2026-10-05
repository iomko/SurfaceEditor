#pragma once

#include "Application.h"
#include <unordered_map>
class Input
{
public:
	static void updateButtonClicks()
    {
        for (auto& [button, clicked] : mouseButtonClicked)
            clicked = false;

        for (auto& [button, released] : mouseButtonReleased)
            released = false;
    }

    static void updateMousePosition()
    {
        double xMousePos, yMousePos;
        glfwGetCursorPos(Application::getWindow().getWindowHandle(), &xMousePos, &yMousePos);
        lastMouseX = xMousePos;
        lastMouseY = yMousePos;
    }
    
    static float getMouseDx()
    {
        double xMousePos, yMousePos;
        glfwGetCursorPos(Application::getWindow().getWindowHandle(), &xMousePos, &yMousePos);
        double mouseXDiff = xMousePos - lastMouseX;
        lastMouseX = xMousePos;
        return mouseXDiff;
    }

    static float getMouseDy()
    {
        double xMousePos, yMousePos;
        glfwGetCursorPos(Application::getWindow().getWindowHandle(), &xMousePos, &yMousePos);
        double mouseYDiff = yMousePos - lastMouseY;
        lastMouseY = yMousePos;
        return mouseYDiff;
    }

    static bool isKeyPressed(int keycode)
    {
        auto keyState = glfwGetKey(Application::getWindow().getWindowHandle(), keycode);
        bool isKeyPressed = keyState == GLFW_PRESS || keyState == GLFW_REPEAT;

        bool wasKeyReleased = !keyStates[keycode] && isKeyPressed;
        keyStates[keycode] = isKeyPressed;

        return wasKeyReleased;
    }

    static bool isKeyDown(int keycode)
    {
        auto keyState = glfwGetKey(Application::getWindow().getWindowHandle(), keycode);
        return keyState == GLFW_PRESS || keyState == GLFW_REPEAT;
    }
    
	static bool isMouseButtonReleased(int button)
    {
        return mouseButtonReleased[button];
    }

    static bool isMouseButtonDown(int button)
    {
        return mouseButtonStates[button];
    }

    static bool isMouseButtonPressed(int button)
	{
		return mouseButtonClicked[button];
	}

    static float getMouseX()
    {
        double xMousePos, yMousePos;
        glfwGetCursorPos(Application::getWindow().getWindowHandle(), &xMousePos, &yMousePos);
        return xMousePos;
    }

    static float getMouseY()
    {
        double xMousePos, yMousePos;
        glfwGetCursorPos(Application::getWindow().getWindowHandle(), &xMousePos, &yMousePos);
        return yMousePos;
    }

public:
    inline static std::unordered_map<int, bool> keyStates;
    inline static std::unordered_map<int, bool> mouseButtonClicked;
    inline static std::unordered_map<int, bool> mouseButtonStates;
    inline static std::unordered_map<int, bool> mouseButtonReleased;

    inline static double lastMouseX = 0.0;
    inline static double lastMouseY = 0.0;
};

