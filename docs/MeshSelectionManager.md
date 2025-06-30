# MeshSelectionManager

The MeshSelectionManager class handles the selection and deselection of entire mesh objects within a scene.
It keeps track of selected meshes, updates their selection state, and ensures consistency by clearing all face selections related to a mesh when it is deselected.

###### Public Methods
`MeshSelectionManager(SelectionHolder& holder)`
Constructs a MeshSelectionManager bound to an external SelectionHolder which stores selected meshes and faces.

`void registerMesh(Mesh* mesh)`
Marks the given mesh as selected.
The mesh is added to the selection vector if it is not already selected, and its internal selection state and index are updated accordingly.

`void unregisterMesh(Mesh* mesh)`
Deselects the given mesh.
If the mesh is currently selected, it is removed from the selection vector efficiently by swapping with the last element and popping the vector.
Additionally, all faces selected within this mesh are deselected and removed from the face selection map to maintain consistency.