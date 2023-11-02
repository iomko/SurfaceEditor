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

void Window::processInput(Camera& camera, float& deltaTime)
{
	processKeys(camera, deltaTime);
	processMouseScroll(camera);
	processLookAtDirection(camera);
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
	glfwSetMouseButtonCallback(windowHandle, Mouse::mouseButtonCallBack);
}

void Window::processLookAtDirection(Camera& camera)
{
	double changedDX = Mouse::getDiffX();
	double changedDY = Mouse::getDiffY();

	if (changedDX != 0.0 || changedDY != 0.0)
	{
		camera.updateCameraDirection(changedDX, changedDY);
	}
}

void Window::processMouseScroll(Camera& camera)
{
	double changedScroll = Mouse::getScrollDiffY();
	if (changedScroll != 0.0)
	{
		camera.updateCameraZoom(changedScroll);
	}
}

void Window::processKeys(Camera& camera, float& deltaTime)
{
	const float movementSpeed = 4.0f * deltaTime;
	if (Keyboard::key(GLFW_KEY_W) == true)
	{
		camera.updateCameraPosition(CameraMovement::FORWARD, movementSpeed);
	}
	if (Keyboard::key(GLFW_KEY_S) == true)
	{
		camera.updateCameraPosition(CameraMovement::BACKWARD, movementSpeed);
	}
	if (Keyboard::key(GLFW_KEY_A) == true)
	{
		camera.updateCameraPosition(CameraMovement::LEFT, movementSpeed);
	}
	if (Keyboard::key(GLFW_KEY_D) == true)
	{
		camera.updateCameraPosition(CameraMovement::RIGHT, movementSpeed);
	}
}


