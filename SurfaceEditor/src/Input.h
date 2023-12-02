#ifndef INPUT_H
#define INPUT_H
#include "Application.h"

class Input
{
public:
	static bool isKeyPressed(int keycode)
	{
		auto state = glfwGetKey(Application::getInstance().getWindow().windowHandle, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	static bool isMouseButtonPressed(int mouse)
	{
		auto state = glfwGetMouseButton(Application::getInstance().getWindow().windowHandle, mouse);
		return state == GLFW_PRESS;
	}
	static float getMouseX()
	{
		double xPos, yPos;
		glfwGetCursorPos(Application::getInstance().getWindow().windowHandle, &xPos, &yPos);
		return xPos;
	}
	static float getMouseY()
	{
		double xPos, yPos;
		glfwGetCursorPos(Application::getInstance().getWindow().windowHandle, &xPos, &yPos);
		return yPos;
	}

};
#endif