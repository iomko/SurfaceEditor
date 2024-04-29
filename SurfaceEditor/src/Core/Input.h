#ifndef INPUT_H
#define INPUT_H


#include "Application.h"
#include <unordered_map>
class Input
{
public:

    static void updateMousePosition()
    {
        double xMousePos, yMousePos;
        glfwGetCursorPos(Application::getWindow().getWindowHandle(), &xMousePos, &yMousePos);
        m_lastMouseX = xMousePos;
        m_lastMouseY = yMousePos;
    } 
    
    static float getMouseDx()
    {
        double xMousePos, yMousePos;
        glfwGetCursorPos(Application::getWindow().getWindowHandle(), &xMousePos, &yMousePos);
        double mouseXDiff = xMousePos - m_lastMouseX;
        m_lastMouseX = xMousePos;
        return mouseXDiff;
    }

    static float getMouseDy()
    {
        double xMousePos, yMousePos;
        glfwGetCursorPos(Application::getWindow().getWindowHandle(), &xMousePos, &yMousePos);
        double mouseYDiff = yMousePos - m_lastMouseY;
        m_lastMouseY = yMousePos;
        return mouseYDiff;
    }

    static bool isKeyDown(int keycode)
    {
        auto keyState = glfwGetKey(Application::getWindow().getWindowHandle(), keycode);
        bool isKeyPressed = keyState == GLFW_PRESS || keyState == GLFW_REPEAT;

        bool wasKeyReleased = !m_keyStates[keycode] && isKeyPressed;
        m_keyStates[keycode] = isKeyPressed;

        return wasKeyReleased;
    }

    static bool isKeyPressed(int keycode)
    {
        auto keyState = glfwGetKey(Application::getWindow().getWindowHandle(), keycode);
        return keyState == GLFW_PRESS || keyState == GLFW_REPEAT;
    }

    static bool isMouseButtonPressed(int mouse)
    {
        auto mouseButtonState = glfwGetMouseButton(Application::getWindow().getWindowHandle(), mouse);
        return mouseButtonState == GLFW_PRESS;
    }

    static bool isMouseButtonClicked(int mouse)
    {
        auto mouseButtonState = glfwGetMouseButton(Application::getWindow().getWindowHandle(), mouse);
        bool isMouseButtonPressed = mouseButtonState == GLFW_PRESS;

        bool wasMouseButtonClicked = !m_mouseButtonStates[mouse] && isMouseButtonPressed;
        m_mouseButtonStates[mouse] = isMouseButtonPressed;

        return wasMouseButtonClicked;
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

private:
    inline static std::unordered_map<int, bool> m_keyStates;
    inline static std::unordered_map<int, bool> m_mouseButtonStates;

    inline static double m_lastMouseX = 0.0;
    inline static double m_lastMouseY = 0.0;
};

#endif