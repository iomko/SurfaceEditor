#ifndef INPUT_H
#define INPUT_H


#include "Application.h"
#include <unordered_map>
class Input
{
public:
	static void resetFrameInput()
	{
		for (auto& [button, clicked] : m_mouseButtonClicked)
			clicked = false;
	}

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

	static bool isMouseButtonClicked(int button)
	{
		return m_mouseButtonClicked[button];
	}

    /*
	static bool isMouseButtonClicked(int button)
	{
		bool clicked = m_mouseButtonClicked[button];
		m_mouseButtonClicked[button] = false;

        if(clicked == true)
        {
            std::cout << "isMouseButtonClicked was Clicked" << std::endl;
        }
		return clicked;
	}
    */

    /*
    static bool isMouseButtonClicked(int mouse)
    {
        auto mouseButtonState = glfwGetMouseButton(Application::getWindow().getWindowHandle(), mouse);
        bool isMouseButtonPressed = mouseButtonState == GLFW_PRESS;

        bool wasMouseButtonClicked = !m_mouseButtonStates[mouse] && isMouseButtonPressed;
        m_mouseButtonStates[mouse] = isMouseButtonPressed;

        if(wasMouseButtonClicked)
        {
            std::cout << "isClicked" << std::endl;
        }

        return wasMouseButtonClicked;
    }
    */

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
    inline static std::unordered_map<int, bool> m_keyStates;
    //inline static std::unordered_map<int, bool> m_mouseButtonStates;
    inline static std::unordered_map<int, bool> m_mouseButtonClicked;

    inline static double m_lastMouseX = 0.0;
    inline static double m_lastMouseY = 0.0;
};

#endif