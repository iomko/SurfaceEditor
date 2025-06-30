# LayerStack

### LayerStack
`LayerStack` manages the collection of layers used by the `Application`.
It allows us to add or remove layer/overlay dynamically.

###### Public Methods

- `void addLayer(Layer* layer)`  
Inserts a `Layer` before all the `Overlays`.
  
- `void removeLayer(Layer* layer)`  
Removes a `Layer` if it exists.
  
- `void addOverlay(Layer* overlay)` 
Adds a `Layer` as an overlay. `Overlays` are inserted after all regular layers added via `addLayer(Layer* layer)` method.

- `void removeOverlay(Layer* overlay)`
Removes an `Overlay` if it exists.

- `std::vector<Layer*>::iterator begin()`  
Returns an iterator to the start of the layer list.

- `std::vector<Layer*>::iterator end()`
Returns an iterator to the end of the layer list.

### Usage

Here is an example of how to use the `LayerStack` class in your project.

`LayerStack` is managed by the `Application` class.

```c++
class Application
{
public:
	...
	// Method that renders and updates all the layers in the LayerStack
	// Regular layers need to be rendered and updated first before all the overlays
	void run()
	{
		m_imGuiLayer->begin();

		for (auto layer : m_layerStack) {
			layer->onImGuiRender();
			layer->onUpdate();
		}
		m_imGuiLayer->end();
	}
	
	// Method that sends events to the Layer class
	// Overlays need to be processed before all the regular layers
	void onEvent(Event& event)
	{
		for (auto layerIt = m_layerStack.end(); layerIt != m_layerStack.begin();)
		{
			(/*--layerIt)->onEvent(event);
			if (event.isHandled)
			{
				break;
			}
		}
	}
	
private:
	LayerStack m_layerStack;
}
```

Layers can now be added to or removed from the `LayerStack` through the `Application` class.

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
	
	...
	// After we are done with the layer and won't need it in future, we can remove it easily
	app.getLayerStack.removeLayer(&myLayer);
}
```