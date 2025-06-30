# FaceSelectionManager
The FaceSelectionManager class manages the selection and deselection of mesh faces within a 3D model.
It keeps track of selected faces per mesh, updates GPU vertex highlighting states, and maintains synchronization between the mesh data and rendering data structures.

###### Public Methods
`FaceSelectionManager(SelectionHolder& holder)`
Constructs a FaceSelectionManager bound to an external SelectionHolder which stores all current selections.

`void registerFace(HalfEdgeDS::Face* face, Mesh* mesh)`
Marks the given face as selected on the specified mesh.
It adds the face to the selection vector associated with the mesh, sets face-related vertices as highlighted for rendering, and updates the face's selection status and index.

`void unregisterFace(HalfEdgeDS::Face* face, Mesh* mesh)`
Removes the given face from the selection on the specified mesh.
It unhighlights the corresponding vertices, removes the face from the selection vector efficiently by swapping with the last element, updates selection indices accordingly, and cleans up the selection container if empty.