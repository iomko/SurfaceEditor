#ifndef WINDOW_H
#define WINDOW_H

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>
#include "Keyboard.h"

class Window
{
public:
	Window(int width, int height, const std::string& title);
	const std::string& screenTitle;
	int screenWidth;
	int screenHeight;
	GLFWwindow* windowHandle;
	bool initialize();
	void terminate();
	void cleanUp();
	void createNewFrame();
	void processInput();
	void setCallBackFunctions();
	void setClosing(bool close);
	void update();

	static void frameBufferSizeCallBack(GLFWwindow* window, int width, int height);
};

#endif
