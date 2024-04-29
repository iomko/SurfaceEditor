#include "../Events/Mouse.h"
#include "Window.h"
#include "Input.h"

import EventSystem;


Window::Window(int width, int height, const std::string& title) : m_screenWidth(width), m_screenHeight(height), m_screenTitle(title){}

void Window::terminate()
{
	glfwTerminate();
}

void Window::clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Window::update()
{
	glfwSwapBuffers(m_windowHandle);
	glfwPollEvents();
}

bool Window::initialize()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_windowHandle = glfwCreateWindow(m_screenWidth, m_screenHeight, m_screenTitle.c_str(), NULL, NULL);

	if (m_windowHandle == NULL)
	{
		return false;
	}
	glfwMakeContextCurrent(m_windowHandle);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return false;
	}

#ifdef __APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glViewport(0, 0, m_screenWidth, m_screenHeight);

	setCallBackFunctions();
	return true;
}

std::string Window::getScreenTitle()
{
	return m_screenTitle;
}

int Window::getScreenWidth()
{
	return m_screenWidth;
}

int Window::getScreenHeight()
{
	return m_screenHeight;
}

GLFWwindow* Window::getWindowHandle()
{
	return m_windowHandle;
}

void Window::setCallBackFunctions()
{
	glfwSetWindowUserPointer(m_windowHandle, this);

	glfwSetFramebufferSizeCallback(m_windowHandle, [](GLFWwindow* window, int width, int height) {
		Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
		instance->m_screenWidth = width;
		instance->m_screenHeight = height;
		WindowResizeEvent event(width, height);

		instance->m_eventFunc(event);
		});

	glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow* window)
		{
			Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent event;
			instance->m_eventFunc(event);
		});

	glfwSetKeyCallback(m_windowHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressEvent event(key);
				instance->m_eventFunc(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleaseEvent event(key);
				instance->m_eventFunc(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressEvent event(key);
				instance->m_eventFunc(event);
				break;
			}
			default:;
			}
		});

	glfwSetCursorPosCallback(m_windowHandle, [](GLFWwindow* window, double xpos, double ypos)
		{
			Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
			MouseMoveEvent event(xpos, ypos);
			instance->m_eventFunc(event);
			Input::updateMousePosition();
		});

	glfwSetScrollCallback(m_windowHandle, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

			MouseScrollEvent event(xoffset, yoffset);
			instance->m_eventFunc(event);
		});

	glfwSetMouseButtonCallback(m_windowHandle, [](GLFWwindow* window, int button, int action, int mods)
		{
			Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressEvent event(button);
				instance->m_eventFunc(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleaseEvent event(button);
				instance->m_eventFunc(event);
				break;
			}
			default:;
			}
		});

}

void Window::setEventFunc(std::function<void(Event&)> func)
{
	m_eventFunc = func;
}

