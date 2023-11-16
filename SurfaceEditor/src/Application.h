#pragma once
#include "Renderer.h"
#include "Window.h"
#include "Event.h"
#include "KeyEvent.h"

class Application
{
public:
	Application(int width, int height, const std::string& title)
		: window(width, height, title)
	{
		window.setEventFunc(std::bind(&Application::onEvent, this, std::placeholders::_1));
		window.initialize();
	}

	// Access the window instance
	Window& getWindow()
	{
		return window;
	}

	void close()
	{
		window.terminate();
	}

	void onEvent(Event& event)
	{
		std::cout << event.getName() << std::endl;
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<KeyPressEvent>([](Event& eventType){std::cout << "OUYEE" << std::endl;});
	}

	void test()
	{
		std::cout << "test" << std::endl;
	}

	void run()
	{
		window.setCallBackFunctions();
		//glfwSetInputMode(window.windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

private:
	Window window;
};
