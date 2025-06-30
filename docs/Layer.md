# Layer

### Layer
`Layer` serves as a base interface for modular application components that need to handle events, perform per-frame updates, and optionally render UI elements.
It is designed to be extended by concrete implementations like `AdditionLayer`, `SelectionLayer`, `RemovalLayer`, and others that participate in the application's runtime behavior and graphical interface.

###### Public Methods

- `Layer(std::string name)`  
Initializes the Layer with the specified name.
  
- `std::string getName()`  
Returns the name of the layer.
  
- `virtual void onEvent(Event& event)`  
Handles incoming events. Events can be marked as handled to prevent propagation.

- `virtual void onImGuiRender()`  
Called once per frame for rendering `ImGui` components.

- `virtual void onUpdate()`  
Called once per frame to update layer logic.

- `virtual void onDetach()`
Called once when the layer is removed from the LayerStack.
Ideal for cleanup tasks, such as releasing resources.
> **Note:** Currently, only `ImGuiLayer` provides a custom implementation of this method.

- `virtual void onAttach()`  
Called once when the layer is added to the `LayerStack`.  
Use this to initialize resources or state specific to the layer.  
> **Note:** Currently, only `ImGuiLayer` provides a custom implementation of this method.

### Usage

Here is an example of how to use the `Layer` class in your project.

First, let's create derived layer class.

```c++
#include "Layer.h"

class MyCustomLayer : public Layer {
public:
    MyCustomLayer(const std::string& name)
        : Layer(name) {}

    void onEvent(Event& event) override
    {
		// Process input
		if (event.getType() == EventType::MouseButtonPress)
		{
			// Check if the mouse cursor is actually inside of the ImGui window
			if (m_isMouseInsideWindow) {
			// Mark event as processed
				event.isHandled = true;
			}
		}		
    }

	void onImGuiRender() override {
		ImGui::Begin(this->getName().c_str());
		
		// Get window position and size
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 windowSize = ImGui::GetWindowSize();
		// Get mouse position
		ImVec2 mousePos = ImGui::GetMousePos();
		
		// Check if the mouse cursor is actually inside of the ImGui window
		m_isMouseInsideWindow = (mousePos.x >= windowPos.x && mousePos.x <= windowPos.x + windowSize.x &&
			mousePos.y >= windowPos.y && mousePos.y <= windowPos.y + windowSize.y);
		
		...
		// Specify the structure of the ImGui window
		...

		ImGui::End();
	}
	
	void onUpdate() override {
		...
		// Layer logic implementation
		...
	}
	
private:
	bool m_isMouseInsideWindow;
};
```

After the creation of the layer class, we'll push the layer to the `LayerStack`.

```c++
#include "Application.h"
#include "MyCustomLayer.h"

int main()
{
	// Initialize the application class
	Application& app = Application::getInstance(SCR_WIDTH, SCR_HEIGHT, "SurfaceEditor");
	
	MyCustomLayer myLayer("MyCustomLayer");
	// Add Layer to the LayerStack
	app.getLayerStack().addLayer(&myLayer);
	
	while (!glfwWindowShouldClose(app.getWindow().getWindowHandle()))
	{
		...
		// Renders and updates the layer implementation logic
		app.run();
	}
}
```