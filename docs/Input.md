# Input

### Input
The `Input` class provides static utility methods to handle user input, including keyboard and mouse interaction.

###### Public Methods

- `static void resetFrameInput()`  
Resets the state of all mouse buttons. It is being called at the end of each frame.

- `static void updateMousePosition()`  
Updates the stored mouse position to the current cursor position.
  
- `static float getMouseDx()`
Returns the distance of a horizontal mouse movement since the last update.
  
- `static float getMouseDy()`  
Returns the distance of a vertical mouse movement since the last update.

- `static bool isKeyPressed(int keycode)`  
Returns `true` if the key was currently pressed.

- `static bool isKeyDown(int keycode)`  
Returns `true` if the key is currently held down.

- `static bool isMouseButtonPressed(int button)`  
Returns `true` if the mouse button was currently pressed.

- `static bool isMouseButtonDown(int mouse)`  
Returns `true` if the mouse button is currently held down.

- `static float getMouseX()`  
Returns the current X position of the mouse cursor.

- `static float getMouseY()`  
Returns the current Y position of the mouse cursor.

### Usage

Here is an example of how to use the `Input` class in your project.

```c++
#include "Input.h"

while (!glfwWindowShouldClose(app.getWindow().getWindowHandle()))
{
	// Render stuff
	
	// Check if key A was just pressed
	if (Input::isKeyPressed(GLFW_KEY_A)){
		// Do something
	}
	
	// Check if key B is held down
	if (Input::isKeyDown(GLFW_KEY_B)){
		// Do something
	}
	
	// Check if the left mouse button was just pressed
	if (Input::isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT)){
		// Do something
	}
	
	// Check if the right mouse button is held down
	if (Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT)){
		// Do something
	}
	
	// Reset the mouse button state
	Input::resetFrameInput();
}
```