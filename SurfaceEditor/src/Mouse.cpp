#include "Mouse.h"

#include <iostream>

double Mouse::positionX = 0.0;
double Mouse::positionY = 0.0;
double Mouse::diffPositionX = 0.0;
double Mouse::diffPositionY = 0.0;
double Mouse::lastPosX = 0.0;
double Mouse::lastPosY = 0.0;
bool Mouse::firstMoved = true;
double Mouse::diffScrollX = 0.0;
double Mouse::diffScrollY = 0.0;

bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = { false };
bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = { false };

void Mouse::cursorPositionCallBack(GLFWwindow* windowHandle, double xPos, double yPos)
{
	if (firstMoved == true)
	{
		Mouse::lastPosX = xPos;
		Mouse::lastPosY = yPos;
		Mouse::positionX = xPos;
		Mouse::positionY = yPos;
		Mouse::diffPositionX = 0.0;
		Mouse::diffPositionY = 0.0;
		firstMoved = false;
	} else
	{
		Mouse::positionX = xPos;
		Mouse::positionY = yPos;
		Mouse::diffPositionX = Mouse::positionX - Mouse::lastPosX;
		Mouse::diffPositionY = Mouse::lastPosY - Mouse::positionY;
		Mouse::lastPosX = Mouse::positionX;
		Mouse::lastPosY = Mouse::positionY;
	}
}

void Mouse::mouseButtonCallBack(GLFWwindow* windowHandle, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (buttons[button] == false)
		{
			buttonsChanged[button] = true;
			buttons[button] = true;
		}
		else
		{
			buttonsChanged[button] = false;
		}
	}
	if (action == GLFW_RELEASE)
	{
		buttons[button] = false;
		buttonsChanged[button] = true;
	}
}

void Mouse::mouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	Mouse::diffScrollX = xoffset;
	Mouse::diffScrollY = yoffset;
}

bool Mouse::buttonPressed(int button)
{
	return buttons[button];
}

bool Mouse::buttonWentDown(int button)
{
	if (buttons[button] == true && buttonsChanged[button] == true)
	{
		return true;
	}
	return false;
}

bool Mouse::buttonWentUp(int button)
{
	if (buttons[button] == false && buttonsChanged[button] == true)
	{
		return true;
	}
	return false;
}

bool Mouse::buttonHasChanged(int button)
{
	return buttonsChanged[button];
}

double Mouse::getDiffX()
{
	double retDiffX = Mouse::diffPositionX;
	Mouse::diffPositionX = 0;
	return retDiffX;
}

double Mouse::getDiffY()
{
	double retDiffY = Mouse::diffPositionY;
	Mouse::diffPositionY = 0;
	return retDiffY;
}

double Mouse::getScrollDiffX()
{
	double retScrollDiffX = Mouse::diffScrollX;
	Mouse::diffScrollX = 0;
	return retScrollDiffX;
}

double Mouse::getScrollDiffY()
{
	double retScrollDiffY = Mouse::diffScrollY;
	Mouse::diffScrollY = 0;
	return retScrollDiffY;
}
