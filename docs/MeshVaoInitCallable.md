# MeshVaoInitCallable
The MeshVaoInitCallable class implements the Callable<MeshParams, void> interface. It initializes vertex array object (VAO) data for a 3D mesh, preparing geometry for rendering. This includes generating triangle data from mesh faces and line data from mesh edges using a half-edge data structure.

###### Public Methods
`void invoke(const MeshParams& input)`
Initializes VAO data for the mesh passed via input.m_mesh. The method performs the following:

- Face processing:
Iterates over all faces in the mesh, sets their material, computes face normals, performs polygon triangulation, and stores the resulting triangles into the renderer's VAO vertex buffer. Each triangle is also recorded in the mesh's internal half-edge face triangle structure.

- Line processing:
Iterates over all edges in the mesh, computes the averaged face normals for each edge (including twin edges if available), and generates two lines slightly offset along the normal direction to represent the top and bottom edges. These are stored in the renderer's line VAO vector.

- Edge indexing:
Each edge is tagged with the index of its associated VAO line data for efficient rendering or lookup.