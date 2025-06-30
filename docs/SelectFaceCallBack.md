# SelectFaceCallBack
The SelectFaceCallBack class implements a callback that performs face selection in the 3D scene using mouse picking.
It utilizes the current camera view and window context to determine the closest mesh face under the cursor and registers it for selection.

This class inherits from Callback<> and Observer, and is typically triggered by a user interaction event (e.g., mouse click).

###### Public Methods
`void execute()`
Performs a face selection operation based on the current camera and mouse position.
It retrieves the closest intersected mesh face using SceneUtilities::retClosestHitData, then extracts the corresponding mesh and face.
The face is then passed to the SelectionController via registerFace, marking it as selected.