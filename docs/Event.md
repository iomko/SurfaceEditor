## Event

The `Event` class defines various types of events that are processed by the [`LayerStack`](boilerplate) and propagated down through the layers. Each `Event` instance holds information about whether it has been handled or not. It also provides methods that must be overridden in derived classes.

###### Public Methods
  - `virtual EventType getType() = 0`
    Returns the type of the event (this method needs to be overriden).

  - `virtual const char* getName() = 0`
    Returns the name of the event (this method needs to be overriden).

### Usage

Here is an example of how to use the `Event` class in your project.

At first, create a derived class from the Event class.
```c++
#include "Event.h"

class MouseButtonPressEvent : public Event
{
public:
	MouseButtonPressEvent(int buttonCode)
	{
		this->m_buttonCode = buttonCode;
	}
	
	// Overridden methods
	virtual EventType getType() override
	{
		return EventType::MouseButtonPress;
	}
	
	virtual const char* getName() override {
		return "mouseButtonPressEvent";
	}
	
	virtual ~MouseButtonPressEvent() override = default;
	
	static EventType getStaticType()
	{
		return EventType::MouseButtonPress;
	}
private:
	int m_buttonCode;
};
```
Once created, it can be used for example inside the glfw callback methods.
```c++
glfwSetMouseButtonCallback(m_windowHandle, [](GLFWwindow* window, int button, int action, int mods)
{
	// Cast to actual Window type
	Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

	// Check for press event
	if(action == GLFW_PRESS){
		if(Input::m_mouseButtonClicked[button] == false)
		{
			Input::m_mouseButtonClicked[button] = true;
		}
		MouseButtonPressEvent event(button);
		instance->m_eventFunc(event);
		Input::m_mouseButtonClicked[button] = false;
	}
});
```

