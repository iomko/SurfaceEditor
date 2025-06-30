## EventType

`EventType` defines the different types of events that can be created and handled within the application.

The application currently supports:

- **Keyboard events** (e.g., key presses, releases)  
- **Mouse events** (e.g., movement, clicks, scrolls)  
- **Window events** (e.g., window resize, close)  

These event types enable your application and its layers to handle user input and respond to window events like resizing or closing.

### Types

- `MouseMove` - Triggered when the mouse is moved.
- `MouseButtonPress` - Triggered when a mouse button is pressed.
- `MouseButtonRelease` - Triggered when a mouse button is released.
- `MouseScroll` - Triggered when a mouse wheel is scrolled.
- `KeyPress` - Triggered when a keyboard key is pressed.
- `KeyRelease` - Triggered when a keyboard key is released.
- `WindowResize` - Triggered when the application window is resized.
- `WindowClose` - Triggered when the application window is requested to close.

### Usage

Here is an example of how to use the `EventType` enum class in your project.

Example of `MouseButtonPress` being used inside the public method of [`AdditionLayer`](boilerplate) class.
```c++
void onEvent(Event& event) override
{
	if (event.getType() == EventType::MouseButtonPress)
	{
		if (m_isMouseInsideWindow) {
			event.isHandled = true;
		}
	}
}
```

