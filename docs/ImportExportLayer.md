# ImportExportLayer

A UI layer for importing and exporting mesh files using ImGui. Integrates with the application's command system and Windows file dialogs.

###### Public Methods:
`ImportExportLayer(const std::string& name, CommandRegistry& commandRegistry)`
Constructor that initializes the layer with a name and reference to the command registry.

`void onEvent(Event& event)`
Handles mouse button press events. If the mouse is inside the layer's ImGui window, marks the event as handled to prevent propagation.

`void onImGuiRender()`
Renders the ImGui interface for import/export operations.
Shows buttons for "Import" and "Export" side by side.