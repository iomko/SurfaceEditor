# FetchedSurfaceVertexGenCallable
The FetchedSurfaceVertexGenCallable class implements the Callable&lt;OpenTopoParams, MeshParams&gt; interface. It fetches a grayscale heightmap from the OpenTopography API, decodes it, and generates a 3D mesh consisting of vertices and triangle indices. This mesh is then returned in the output parameter.

###### Public Methods
`void invoke(const OpenTopoParams& input, MeshParams& output)`
Downloads a .tif file using a URL constructed from OpenTopoParams, parses the heightmap, generates a vertex grid, constructs triangle indices, and creates a new Mesh object assigned to output.m_mesh.