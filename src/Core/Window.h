#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Event.h"
#include "../Scene/Camera.h"

class Window
{
public:
	Window(int width, int height, const std::string& title);
	void terminate();
	void clearColorBuffer(float r, float g, float b, float a);
	void clearDepthBuffer();
	void clearStencilBuffer();
	void clearAllBuffers(float r, float g, float b, float a);
	void update();
	bool initialize();
	std::string getScreenTitle();
	int getScreenWidth();
	int getScreenHeight();
	GLFWwindow* getWindowHandle();
	void setEventFunc(std::function<void(Event&)> func);
private:
	std::string m_screenTitle;
	int m_screenWidth;
	int m_screenHeight;
	GLFWwindow* m_windowHandle = nullptr;
	std::function<void(Event& event)> m_eventFunc = nullptr;
	void setCallBackFunctions();
};

#endif
