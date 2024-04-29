#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string>
#include "../Events/Keyboard.h"

import EventSystem;
import Camera;

class Window
{
public:
	Window(int width, int height, const std::string& title);
	void terminate();
	void clear();
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
