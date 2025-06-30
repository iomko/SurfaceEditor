# SelectionLayerCallBack
The SelectionLayerCallBack class handles logic for selecting and deselecting scene elements (faces, edges, vertices, and objects) based on input parameters.
It is designed to switch between different selection tools and clean up tool-specific parameters accordingly.

This class inherits from Callback<SelectionLayerParams> and Observer, and is used to manage selection state transitions in the application's tool system.

###### Public Methods
`void execute(const SelectionLayerParams& params)`
Based on the SelectionLayerParams, this method configures the current selection or deselection tool inside the viewport controller.
It reads two key parameters:

params.m_selectionMode - defines the selection target (Face, Edge, Vertex, Object).

params.m_type - specifies whether it's a Selection or Deselection operation.

Depending on the combination of these parameters, the method retrieves and assigns the appropriate tool from the ToolRegistry and updates ViewPortsHolderContext::s_viewPortsController->m_currentTool.