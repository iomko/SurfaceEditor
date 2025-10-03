# FaceSelectionManager
The FaceSelectionManager class manages the selection and deselection of mesh faces within a 3D model.

### FaceSelectionManager class

###### Methods
`FaceSelectionManager(SelectionHolder& holder)` - Constructs a FaceSelectionManager bound to an external SelectionHolder which stores all current selections.  

`void registerFace(HalfEdgeDS::Face* face, Mesh* mesh)` - Marks the given face as selected on the specified mesh. It adds the face to the selection vector associated with the mesh.  
`void unregisterFace(HalfEdgeDS::Face* face, Mesh* mesh)` - Removes the given face from the selection on the specified mesh. It removes the face from the selection vector associated with the mesh.  

