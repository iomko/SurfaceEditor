# DeselectFaceCallBack
This class implements a callback to deselect a specific face in the scene.
It inherits from Callback<> and Observer.
When executed, it finds the face currently under the mouse cursor and removes it from the selection.

###### Public Methods
`void execute()`
Executes the deselection process by:

retrieving the active camera and window,

performing a raycast to find the closest mesh face hit under the cursor,

obtaining the mesh and face from the hit data,

unregistering the face from the selection controller to remove it from the current selection.