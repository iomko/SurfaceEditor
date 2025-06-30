# PlaneVertexGenCallable
The PlaneVertexGenCallable class implements the Callable<PlaneParams, MeshParams> interface. It procedurally generates a flat, subdivided plane mesh composed of triangles and outputs it via the MeshParams structure.

###### Public Methods
`void invoke(const PlaneParams& input, MeshParams& output)`
Generates a plane mesh based on input parameters and returns it in the output. The process includes:

- Vertex generation:
Computes grid-aligned vertex positions for a square plane centered at the origin. The density of the grid is determined by the m_subdivisionLevel parameter.

- Triangle construction:
For each quad in the vertex grid, two triangles are formed by calculating the appropriate indices. This results in a uniform tessellation of the plane.

- Mesh creation:
Constructs a Mesh object using the generated vertices and triangle indices, with a default material and shader. The mesh is assigned to output.m_mesh.

`Input (PlaneParams)`
float m_size: Total width and depth of the square plane.

int m_subdivisionLevel: Number of subdivisions along each axis (e.g., 1 results in 1 quad made of 2 triangles, 2 results in 4 quads, etc.).

`Output (MeshParams)`
Mesh* m_mesh: Pointer to the newly created mesh representing the plane.