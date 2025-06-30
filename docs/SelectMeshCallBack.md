# SelectMeshCallBack
The SelectMeshCallBack class is a callback used for selecting a mesh in the 3D scene based on user interaction (e.g., mouse click).
It determines which mesh was hit by the picking ray (from the camera through the cursor) and registers it through the selection system.

This class inherits from Callback<> and Observer.

###### Public Methods
`void execute()`
Performs a hit test in the current scene using the camera and window context.
If a mesh is detected under the cursor, it is registered as selected via the SelectionController.