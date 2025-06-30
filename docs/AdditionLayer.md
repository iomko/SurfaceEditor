# AdditionLayer

A GUI layer class integrating ImGui interface for adding objects and fetching surface data.

Inherits from Layer, Observable, and Observer. Uses CommandRegistry to execute commands triggered by UI actions.

###### Public Methods:
`AdditionLayer(const std::string& name, CommandRegistry& commandRegistry)`
Constructor initializing the layer with a name and a reference to the CommandRegistry.

`void onEvent(Event& event)`
Handles events, specifically intercepting mouse button press events when the mouse is inside the window to mark the event as handled.

`void onImGuiRender()`
Renders the ImGui window for this layer, including UI for adding mesh objects and fetching surface data from OpenTopography via API key and coordinate inputs.