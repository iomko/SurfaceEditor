#include "Mouse.h"
#include "Window.h"


Window::Window(int width, int height, const std::string& title)
	:screenWidth(width), screenHeight(height), screenTitle(title)
{
}

bool Window::initialize()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->windowHandle = glfwCreateWindow(this->screenWidth, this->screenHeight, this->screenTitle.c_str(), NULL, NULL);

	if (this->windowHandle == NULL)
	{
		return false;
	}
	glfwMakeContextCurrent(this->windowHandle);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return false;
	}

#ifdef __APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glViewport(0, 0, 800, 600);
	return true;
}

void Window::terminate()
{
	glfwTerminate();
}

void Window::cleanUp()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::createNewFrame()
{
	glfwSwapBuffers(this->windowHandle);
	glfwPollEvents();
}

void Window::processInput()
{
	
}

void Window::frameBufferSizeCallBack(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Window::setCallBackFunctions()
{
	glfwSetKeyCallback(windowHandle, Keyboard::keyCallback);
	glfwSetCursorPosCallback(windowHandle, Mouse::cursorPositionCallBack);
	glfwSetFramebufferSizeCallback(windowHandle, frameBufferSizeCallBack);
	glfwSetScrollCallback(windowHandle, Mouse::mouseScrollCallBack);
}

void Window::setClosing(bool close)
{

}

void Window::update()
{
}
