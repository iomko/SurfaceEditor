# SelectionController

The SelectionController class provides a unified interface to manage selection states of both meshes and individual faces within those meshes.

### SelectionController class

###### Methods
`SelectionController()` - Constructs the SelectionController, initializing the internal MeshSelectionManager and FaceSelectionManager with a shared SelectionHolder.  
`const SelectionHolder& getHolder() const` - Returns a SelectionHolder.  
`void registerFace(Mesh* mesh, HalfEdgeDS::Face* face)` - Registers (selects) the specified face within the given mesh.  
`void unregisterFace(Mesh* mesh, HalfEdgeDS::Face* face)` - Unregisters (deselects) the specified face within the given mesh.  
`void registerMesh(Mesh* mesh)` - Registers (selects) the specified mesh.  
`void unregisterMesh(Mesh* mesh)` - Unregisters (deselects) the specified mesh.  
