# DeleteSelectedMeshesCallBack
This class defines a callback responsible for deleting all selected meshes in the scene.
It inherits from Callback<> (a parameterless callback) and Observer.
When executed, it iterates through all selected meshes and performs the deletion process.

###### Public Methods
`void execute()`
Executes the deletion of all selected meshes by:

removing each mesh from the scene's octrees,

deleting the mesh data from the VAO and half-edge structure,

unregistering the mesh from the selection controller.
This process continues until no selected meshes remain.