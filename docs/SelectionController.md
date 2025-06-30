# SelectionController

The SelectionController class provides a unified interface to manage selection states of both meshes and individual faces within those meshes.
It internally coordinates MeshSelectionManager and FaceSelectionManager instances, sharing a common SelectionHolder that stores the current selection state.

###### Public Methods
`SelectionController()`
Constructs the SelectionController, initializing the internal MeshSelectionManager and FaceSelectionManager with a shared SelectionHolder.

`const SelectionHolder& getHolder() const`
Returns a constant reference to the internal SelectionHolder, which holds the current selection data for meshes and faces.

`void registerFace(Mesh* mesh, HalfEdgeDS::Face* face)`
Registers (selects) the specified face within the given mesh. Delegates to the internal FaceSelectionManager.

`void unregisterFace(Mesh* mesh, HalfEdgeDS::Face* face)`
Unregisters (deselects) the specified face within the given mesh. Delegates to the internal FaceSelectionManager.

`void registerMesh(Mesh* mesh)`
Registers (selects) the specified mesh. Delegates to the internal MeshSelectionManager.

`void unregisterMesh(Mesh* mesh)`
Unregisters (deselects) the specified mesh. Delegates to the internal MeshSelectionManager.