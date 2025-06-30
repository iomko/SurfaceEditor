# ViewPortsController

### ViewPortsController
The central class responsible for managing multiple ViewPortLayer instances. It tracks active layers, the active tool, the scene, and a command queue.

###### Public Methods:
`ViewPortsController()`
Default constructor that initializes the controller.

`void addLayer(ViewPortLayer* viewPortLayer)`
Adds a new ViewPortLayer pointer to the managed layers list.

### ViewPortsHolderContext
A global singleton-style static context holding references to key objects used across the view port system.

### ViewPortLayer
Represents a renderable and interactive viewport layer. Manages its own camera and interaction logic, including camera movement and tool application.

###### Public Methods:

`ViewPortLayer(const std::string& name)`
Constructs the viewport layer with a given name. Initializes OpenGL renderer and checks framebuffer completeness.

`void onUpdate()`
Called each frame to update camera movement.

`void onEvent(Event& event)`
Handles input events for camera control and tool interaction.

`void updateCameraDirection(Event& event)`
Handles middle mouse button drag to update the camera's view direction.

`void updateCameraMovement()`
Checks keyboard input and moves the camera accordingly.