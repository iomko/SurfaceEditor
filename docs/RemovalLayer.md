# RemovalLayer
This layer provides a simple ImGui interface for deleting selected elements in the scene: either selected faces or selected meshes. It hooks into the application's command system to trigger the respective delete commands.

###### Public Methods:
`RemovalLayer(const std::string& name, CommandRegistry& commandRegistry)`
Constructor that initializes the layer with a name and a reference to the central command registry.

`void onEvent(Event& event)`
Handles mouse button press events. If the mouse cursor is inside this layer's ImGui window, it marks the event as handled to prevent further processing by other layers or the application.

`void onImGuiRender()`
Renders the ImGui window for this layer. It:

Displays two buttons:

Delete Selected Faces - on click, executes the DeleteSelectedFacesCommand.

Delete Selected Meshes - on click, executes the DeleteSelectedMeshesCommand.