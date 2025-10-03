# MeshSelectionManager

The MeshSelectionManager class handles the selection and deselection of entire mesh objects within a scene.

### MeshSelectionManager class

###### Methods
`MeshSelectionManager(SelectionHolder& holder)` - Constructs a MeshSelectionManager bound to an external SelectionHolder which stores selected meshes and faces.

`void registerMesh(Mesh* mesh)` - Marks the given mesh as selected. The mesh is added to the selection vector.
`void unregisterMesh(Mesh* mesh)` - Deselects the given mesh. It removes the mesh from the selection vector.
