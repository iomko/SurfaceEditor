# ImportMeshesCallback
The ImportMeshesCallback class is a callback responsible for loading OBJ mesh files and integrating the imported meshes into the scene. It builds rendering data and spatially organizes mesh faces using octree structures for efficient access and rendering.

###### Public Methods
`void execute(const ImportExportMeshesParams& params)`
Loads an OBJ file from the specified path (params.m_filePathMeshes).
For each imported mesh, it creates the necessary rendering data. Then, for each face of the mesh, it computes a bounding box and assigns the face to the appropriate voxels in the scene's octree structures (coordsOctreeMap).