#ifndef MOUSE_H
#define MOUSE_H

#include <glad\glad.h>
#include <GLFW\glfw3.h>

class Mouse
{
public:
	static double positionX;
	static double positionY;
	static double diffPositionX;
	static double diffPositionY;

	static double diffScrollX;
	static double diffScrollY;

	static double lastPosX;
	static double lastPosY;
	static bool firstMoved;
	static bool buttons[];
	static bool buttonsChanged[];
public:
	static double getDiffX();
	static double getDiffY();

	static double getScrollDiffX();
	static double getScrollDiffY();

	static bool buttonHasChanged(int button);
	static bool buttonWentDown(int button);
	static bool buttonWentUp(int button);
	static bool buttonPressed(int button);
	static void cursorPositionCallBack(GLFWwindow* windowHandle, double xPos, double yPos);
	static void mouseButtonCallBack(GLFWwindow* windowHandle, int button, int action, int mods);
	static void mouseScrollCallBack(GLFWwindow* window, double xoffset, double yoffset);

};

#endif