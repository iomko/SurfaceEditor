# DeselectMeshCallBack
This class defines a callback that deselects a mesh currently under the mouse cursor in the 3D scene.
It inherits from Callback<> and Observer.
When executed, it uses the camera and window context to detect the closest mesh under the cursor and removes it from the selection.

###### Public Methods
`void execute()`
Performs the following steps when called:

Retrieves the current camera, window, and scene objects,

Uses raycasting to find the closest mesh and face hit under the cursor,

Extracts the mesh from the hit data,

Unregisters the mesh from the selection controller, effectively deselecting it.