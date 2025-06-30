# SculptToolsLayer

A UI layer providing ImGui controls for selecting and configuring sculpting tools, specifically a brush tool, within the viewport system.

Handles mouse event interception when the mouse is over the layer's ImGui window and integrates with the viewport controller to set the current active tool and its parameters.

###### Public Methods:
`SculptToolsLayer(const std::string& name)`
Constructor initializing the layer with a given name. Does not initialize brush parameters by default.

`void onEvent(Event& event)`
Handles input events; intercepts mouse button press events when the mouse is inside the ImGui window to prevent event propagation.

`void onImGuiRender()`
Renders the ImGui interface for the layer, including:

Sliders to adjust brush strength and brush radius, which update the parameters passed to the tool.