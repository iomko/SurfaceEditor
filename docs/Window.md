# Window

### Window
The `Window` class encapsulates the creation and management of a GLFW-based window along with an OpenGL rendering context.
It handles window initialization, forwards input events (like keyboard, mouse, and window events) to the `Application` through a user-defined callback. It also manages buffer swapping throughout the application's lifecycle.

###### Public Methods

- `Window(int width, int height, const std::string& title)`  
Constructs the window with the given dimensions and title. The window is not initialized until `initialize()` method is called.
  
- `void terminate()`  
Properly shuts down GLFW and cleans up window resources.
  
- `void clearColorBuffer(float r, float g, float b, float a)`  
Clears the color buffer with the specified RGBA color values.
This effectively fills the entire window with the given color.

- `void clearDepthBuffer()`  
Clears the depth buffer, which resets all stored depth information.
This ensures correct depth testing for new frame rendering.

- `void clearStencilBuffer()`  
Clears the stencil buffer. Useful for masking and multi-pass effects.

- `void clearAllBuffers(float r, float g, float b, float a)`  
Clears color, depth, and stencil buffers. It also fills the entire window with the  given color.

- `void update()`  
Swaps the front and back buffers to display new rendered frame. It also polls input events such as keyboard, mouse, or window events.

- `bool initialize()`  
Initializes GLFW, creates the window and OpenGL context. It also sets up all the internal input callbacks.
Returns true if the initialization succeeded.

- `std::string getScreenTitle()`  
Returns the title of the window.

- `int getScreenWidth()`  
Returns the current window width.

- `int getScreenHeight()`  
Returns the current window height.

- `GLFWwindow* getWindowHandle()`  
Returns a pointer to the underlying GLFW window handle.

- `void setEventFunc(std::function<void(Event&)> func)`  
Sets the callback function that will be invoked when an event (like key press or window resize) occurs.

### Usage

Here is an example of how to use the `Window` class in your project.

First the `Window` class needs to be created and initialized.
In this example, the `Application` class is responsible for the creation and initialization.

```c++
#include "Window.h"

class Application
{
	Application(int width, int height, const std::string& title)
	{
		// Create window
		m_window = new Window(width, height, title);
		// Binds the callback function that will be executed once an event occurs
		m_window->setEventFunc(std::bind(&Application::onEvent, this, std::placeholders::_1));
		m_window->initialize();
		...
		// Makes the mouse cursor visible
		glfwSetInputMode(m_window->getWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

};
```

Once the window is successfully created and initialized, multiple GLFW callbacks are registered to handle various input and window events. Here each callback forwards the corresponding event to the `onEvent` function of the `Application` class, which centralizes event processing.

For example, the following callback captures mouse movement and forwards it as a `MouseMoveEvent` to the `onEvent` handler in `Application`.
(This callback was registered inside the `setCallBackFunctions()` method of the `Window` class.)

```c++
void setCallBackFunctions(){
	...
	glfwSetCursorPosCallback(m_windowHandle, [](GLFWwindow* window, double xpos, double ypos)
	{
		Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
		MouseMoveEvent event(xpos, ypos);
		instance->m_eventFunc(event);
		Input::updateMousePosition();
	});
	...
}
```

We also use the `update()` method of the `Window` class to poll for new events, aswell as for swapping the front and back buffers to display new rendered frame.

```c++
int main(){
	...
	while (!glfwWindowShouldClose(app.getWindow().getWindowHandle()))
	{
		...
		app.getWindow().update();
		...
	}
	...
}
```