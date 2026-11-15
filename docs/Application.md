## Application

The Application class is the main entry point of the entire application. It manages all layers within the application, collects events, and updates GUI windows.

### Application class

###### Methods:

`static Application& getInstance(int width = 800, int height = 600, const std::string& title = "Application")` – Returns the instance of the Application class.  

`static Window& getWindow()` – Retrieves the application window.  

`void close()` – Deallocates the created window.  

`LayerStack& getLayerStack()` – Returns the list of all layers currently registered in the application.  

`ModelObjectRepository& getModelObjectRepository()` – Returns the application-owned repository for creating and managing model objects. The repository is cleared during application shutdown.

`void onEvent(Event& event)` – Handles events.  

`void run()` – Updates the GUI windows.  

The application owns the `ModelObjectRepository`, which owns the `ModelObject`
instances and, through them, their attached meshes. This lifetime is
independent of `Scene`; the scene manages mesh spatial indices rather than
model-object ownership. See [ModelObjectRepository](ModelObjectRepository.md)
and [ModelObject](ModelObject.md).
