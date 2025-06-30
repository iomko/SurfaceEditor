# SceneMeshAdderCallable
The SceneMeshAdderCallable class implements the Callable<MeshParams, void> interface. It inserts a mesh's faces into the scene's spatial octrees, but only if the mesh has not already been added.

###### Public Methods
`void invoke(const MeshParams& input)`
Adds all faces of the mesh into the active scene's octrees. The method performs the following:

- Scene retrieval:
Accesses the current scene through ViewPortsHolderContext.

- Existence check:
Verifies whether the mesh already exists in the scene's meshFaceOctreeCoordsMap. If it does, the insertion is skipped.

- Face insertion:
Iterates over all faces of the mesh and inserts them into the scene's octrees using scene->addFaceIntoOctrees.

`Input (MeshParams)`
Mesh* m_mesh: Pointer to the mesh whose faces should be added to the scene's octree structure.