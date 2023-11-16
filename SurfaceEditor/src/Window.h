#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>
#include "Keyboard.h"
#include "Camera.h"
#include "Event.h"

class Window
{
public:
	Window(int width, int height, const std::string& title);
	const std::string& screenTitle;
	int screenWidth;
	int screenHeight;
	GLFWwindow* windowHandle;
	std::function<void(Event& event)> eventFunc;
	void setEventFunc(std::function<void(Event&)> func);
	void terminate();
	void cleanUp();
	void createNewFrame();
	void processInput(Camera& camera, float& deltaTime);
	void setCallBackFunctions();
	static void frameBufferSizeCallBack(GLFWwindow* window, int width, int height);
	bool initialize();
private:
	void processLookAtDirection(Camera& camera);
	void processMouseScroll(Camera& camera);
	void processKeys(Camera& camera, float& deltaTime);
};

#endif
