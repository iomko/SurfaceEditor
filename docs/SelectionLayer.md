# SelectionLayer

A UI layer that provides ImGui controls for selecting the active selection mode and selection type within an application.

Manages user input for choosing among face, edge, vertex, or object selection modes, as well as toggling between selection and deselection types. Notifies observers of any changes to the selection parameters.

###### Public Methods:
`SelectionLayer(const std::string& name)`
Constructor initializing the layer with a specified name. Defaults selection mode to Face and type to Selection.

`SelectionLayerParams::SelectionMode getSelectionMode() const`
Returns the current selection mode (Face, Edge, Vertex, or Object).

`SelectionLayerParams::Type getType() const`
Returns the current selection type (Selection or Deselection).

`void onImGuiRender()`
Renders the ImGui interface for the selection controls, including:

Radio buttons to toggle between selection and deselection types.