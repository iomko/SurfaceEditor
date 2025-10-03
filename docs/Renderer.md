# Renderer

### RendererBuffersData class

Defines the GPU-side vertex formats used by the renderer.  

`AABBVertex`: Holds position (glm::vec3) and color (glm::vec3).  

`LineVertex`: Holds position (glm::vec3) and a highlight flag (float).  

`PointVertex`: Holds position (glm::vec3) and a highlight flag (float).  

`MeshVertex`: Holds position (glm::vec3), normal (glm::vec3), and a highlight flag (float).  

### BufferStorage class

The renderer uses a generic buffer storage system to manage raw vertex data for different geometry types (meshes, lines, points, AABBs, and printable meshes).  

Each buffer storage provides the following core functionality:

`getBufferData(...)` — Retrieves buffer data if it exists for a given object.  

`updateBufferStorage(...)` — Updates GPU buffer data from CPU-side vertices.  

`registerBufferStorage(...)` — Allocates and initializes GPU buffer objects (VAO, VBO) for a given object if not already present.  

`static std::string getBufferStorageName()` — Returns a unique name identifying the buffer storage types.  


###### Available buffer storages:

`MeshBufferStorage` — Stores triangle mesh geometry (with materials).  

`LineBufferStorage` — Stores line-based geometry for meshes.  

`PointBufferStorage` — Stores per-vertex point data for meshes.  

`AABBBufferStorage` — Stores box geometry for axis-aligned bounding boxes.  

`PrintableMeshBufferStorage` — Stores line geometry for printable/debug meshes.  

All storages are registered in a global BufferRegistry managed by the Renderer.


### Renderer class

The Renderer class provides functions to draw various scene elements using the registered buffer storages.

###### Methods:

`static void init()` - Initializes the buffer registry by registering all supported buffer storage types.  
`static void drawMeshPoints(Mesh* mesh, Shader* shader)` - Draws mesh vertices.  
`static void drawMeshLines(Mesh* mesh, Shader* shader)` - Draws mesh lines.  
`static void drawPrintableMesh(PrintableMesh* mesh, Shader* shader)` - Draws a PrintableMesh as lines.  
`static void drawMesh(Mesh* mesh)` - Draws mesh faces.  
`static void drawBox(const AABBBoundingRegion& aabb)` - Draws an AABB as a wireframe box.  
