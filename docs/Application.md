## Application

The `Application` class serves as the **entry point** of the entire application.  
It ties together essential components like [`LayerStack`](boilerplate), [`ImGuiLayer`](boilerplate), and [`Window`](boilerplate).

Upon construction, it creates and initializes the main `Window`, sets up the `ImGuiLayer`, and adds it as an overlay to the `LayerStack`, which manages all the application layers.

After the `Application` is initialized, other layers can be additionally added to the `LayerStack`. These layers are updated and rendered inside the `run()` method.

The class is also responsible for handling events through the `onEvent(Event& event)` method. [`Event`](boilerplate) is passed from top to bottom through the layers. Once a layer handles the event, propagation stops.

Finally, when the application shuts down, the `close()` method is called via the destructor, which ensures that the `Window` is properly terminated and resources are cleaned up.

###### Public Methods
  - `static Application& getInstance(int width = 800, int height = 600, const std::string& title = "Application")`
    Returns the singleton instance of the application. If it's not created yet, it initializes it with the given Window size and title.

  - `static Window& getWindow()`
    Returns a reference to the main application window. Throws an `std::runtime_error` exception if the window is not yet initialized.

  - `void close()` 
    Terminates and destroys the application window, cleaning up resources.

  - `LayerStack& getLayerStack()`
    Provides access to the `LayerStack` where layers can be added or queried.

  - `void onEvent(Event& event)`
    Distributes an event to the layers from top to bottom until one of them handles it.
	
  - `void run()`
	Updates all the gui components. This includes calling `onUpdate()` and `onImGuiRender()` on each `Layer`.

### Usage

Here is an example of how to use the `Application` class in your project.

```c++
#include "Application.h"
#include "MyCustomLayer.h"

int main()
{
    // Create the application instance
    Application& app = Application::getInstance(1280, 720, "Surface Editor");
	
	// Create your own layer
	MyCustomLayer* myCustomLayer = new MyCustomLayer("MyCustomLayer");
	
	// Add your own layer to the LayerStack
    app.getLayerStack().addLayer(myCustomLayer);

	// Run the main loop
	while (!glfwWindowShouldClose(app.getWindow().getWindowHandle()))
	{
		//Draw stuff
		...
		
		//Later you could even remove the layer, if you won't need it (in if condition or such)
		app.getLayerStack().removeLayer(myCustomLayer);
		delete myCustomerLayer;

		app.run();
		app.getWindow().update();
	}
	
    return 0;
}
```