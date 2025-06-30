
# DeleteSelectedFacesCallBack
This class represents a callback for deleting selected faces in the scene.
It inherits from Callback<> (a parameterless callback) and Observer.
When executed, it removes all selected faces from the half-edge data structure, scene octrees, and selection lists.

###### Public Methods
`void execute()`
The main method performing the callback action.
For each selected mesh, it iterates over all selected faces and:

deletes them from the scene's octrees,

removes their data from the half-edge structure and VAO,

unregisters them from the selection controller.
If a mesh has no remaining faces after deletion, it unregisters the entire mesh from the selection controller.

