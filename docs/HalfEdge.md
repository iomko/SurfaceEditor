# HalfEdge

### GraphEdge
Represents an edge in the graph view, linking vertices and faces.

### HalfEdge
Represents a half-edge in the half-edge data structure used for polygon mesh representation.

Provides connectivity to adjacent half-edges, vertex, edge, and face.

###### Public Methods:

`HalfEdge(HalfEdgeMesh& mesh)` - Constructor initializing reference to the parent mesh.

### Vertex
Represents a vertex in the half-edge mesh.

Stores position and pointer to one outgoing half-edge.

Keeps lists of adjacent edges and graph edges.

###### Public Methods:

`Vertex(HalfEdgeMesh& mesh)` - Constructor initializing reference to the parent mesh.

### Edge
Represents an edge consisting of two half-edges.

Holds pointers to the two vertices it connects.

###### Public Methods:

`Edge(HalfEdgeMesh& mesh)` - Constructor initializing reference to the parent mesh.

### Face
Represents a face in the half-edge structure.

Contains a pointer to one half-edge on its boundary.

Provides nested iterator classes to iterate over the face's vertices and half-edges.

Stores selection status, indices, material pointer, and graph edges.

###### Public Methods:

Iterators:
`FaceHalfEdgeIterator faceHalfEdgeBegin()` - Returns iterator to the first half-edge of the face.

`FaceHalfEdgeIterator faceHalfEdgeEnd()` - Returns iterator to one past the last half-edge.

`FaceVertexIterator faceVertexBegin()` - Returns iterator to the first vertex of the face.

`FaceVertexIterator faceVertexEnd()` - Returns iterator to one past the last vertex.

### Face::FaceVertexIterator
Iterator over the vertices of a face.

###### Public Methods:

`FaceVertexIterator(HalfEdge* halfEdge)` - Constructor.

`FaceVertexIterator& operator++()` - Increment iterator.

`Vertex& operator*()` - Dereference to get current vertex.

`bool operator==(const FaceVertexIterator&) const` - Equality comparison.

`bool operator!=(const FaceVertexIterator&) const` - Inequality comparison.

### Face::FaceHalfEdgeIterator
Iterator over the half-edges of a face.

###### Public Methods:

`FaceHalfEdgeIterator(HalfEdge* halfEdge)` - Constructor.

`FaceHalfEdgeIterator& operator++()` - Increment iterator.

`HalfEdge& operator*()` - Dereference to get current half-edge.

`bool operator==(const FaceHalfEdgeIterator&) const` - Equality comparison.

`bool operator!=(const FaceHalfEdgeIterator&) const` - Inequality comparison.

### FaceTriangle
Struct representing a triangle within a face.

### HalfEdgeMesh
The main class representing the entire half-edge mesh.

Stores all half-edges, vertices, edges, and faces.

Maintains a map of materials to face triangles.

###### Public Methods:

Iterators to internal containers:
`auto halfEdgeIterBegin()`

`auto halfEdgeIterEnd()`

`auto vertexIterBegin()`

`auto vertexIterEnd()`

`auto edgeIterBegin()`

`auto edgeIterEnd()`

`auto faceIterBegin()`

`auto faceIterEnd()`

Topology and data-related methods:
`void getVerticesFromFace(Face* face, std::vector<Vertex>& vertices)`
Retrieves all vertices belonging to the specified face.

Template methods for creating and finding objects:
`template <typename T, typename Vector> T* createObject(Vector& storageVector)`
Creates a new mesh element (vertex, half-edge, face, edge) and adds it to the storage vector.

`template <typename T, typename Key, typename Map> T* findObject(const Key& key, Map& helperMap)`
Finds an object by key in a helper map.

`template <typename T, typename Key, typename Map, typename Vector> T* findOrCreateObject(const Key& key, Map& helperMap, Vector& storageVector, bool* outWasCreated = nullptr)`
Finds or creates an object.

Methods for building the mesh structure:
`void buildGraph()`
Builds the graph representation of the mesh.

`void build(const std::vector<std::vector<int>>& polygons, const std::vector<glm::vec3>& vertices)`
Builds the half-edge structure from input polygons and vertex positions.
