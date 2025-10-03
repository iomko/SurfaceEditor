# HalfEdgeDS

The Half-Edge Data Structure (HalfEdgeDS) is a mesh representation commonly used in computational geometry and computer graphics.
It represents a polygonal mesh by explicitly storing connectivity information between vertices, edges, faces, and half-edges.
This allows efficient traversal, neighborhood queries, and modifications of the mesh topology.

### HalfEdge

Represents a directed edge in the half-edge data structure.
Each edge is split into two opposite half-edges, which allow traversal between adjacent faces.

###### Members:

`Traits::HType* m_next` – Pointer to the next half-edge in the same face loop.

`Traits::HType* m_previous` – Pointer to the previous half-edge in the same face loop.

`Traits::HType* m_twin` – Pointer to the opposite half-edge (the other side of the edge).

`Traits::VType* m_vertex` – The vertex at the start of this half-edge.

`Traits::EType* m_edge` – The undirected edge this half-edge belongs to.

`Traits::FType* m_face` – The face bounded by this half-edge.

`Traits::DerType& m_mesh` – Reference to the owning mesh.

`int m_halfEdgeIndexInVector` – Index of the half-edge in storage (for bookkeeping).

###### Methods:

`HalfEdge(Traits::DerType& mesh)` – Constructor initializing with reference to parent mesh.

`HalfEdge()` – Default constructor.

#### Vertex

Represents a mesh vertex storing its position and an incident half-edge.

###### Members:

`Traits::HType* m_halfEdge` – An outgoing half-edge from this vertex.

`glm::vec3 m_position` – 3D position of the vertex.

`Traits::DerType& m_mesh` – Reference to the owning mesh.

`int m_vertexIndexInVector` – Index of the vertex in storage.

###### Methods:

`Vertex(Traits::DerType& mesh)` – Constructor initializing with reference to parent mesh.

#### Edge

Represents an undirected edge in the mesh, composed of two half-edges.

###### Members:

`Traits::HType* m_halfEdge` – A half-edge associated with this edge.

`Traits::VType* m_firstVertex` – One endpoint of the edge.

`Traits::VType* m_secondVertex` – The other endpoint of the edge.

`int m_edgeIndexInVector` – Index of the edge in storage.

`bool m_isOuter` – Flag marking whether this edge lies on the mesh boundary.

`Traits::DerType& m_mesh` – Reference to the owning mesh.

###### Methods:

`Edge(Traits::DerType& mesh)` – Constructor initializing with reference to parent mesh.

#### Face

Represents a polygonal face in the mesh, defined by a loop of half-edges.

###### Members:

`Traits::HType* m_halfEdge` – A half-edge belonging to this face.

`Traits::DerType& m_mesh` – Reference to the owning mesh.

`int m_faceIndexInVector` – Index of the face in storage.

###### Methods:

`Face(Traits::DerType& mesh)` – Constructor initializing with reference to parent mesh.

`FaceVertexIterator faceVertexBegin()` / `faceVertexEnd()` – Iterators for traversing vertices of the face.

`FaceHalfEdgeIterator faceHalfEdgeBegin()` / `faceHalfEdgeEnd()` – Iterators for traversing half-edges of the face.

#### FaceVertexIterator

An iterator for looping over vertices of a face using its half-edge cycle.

#### FaceHalfEdgeIterator

An iterator for looping over half-edges of a face.

#### HalfEdgeMesh

Main mesh class that manages and stores half-edges, vertices, edges, and faces.
Provides methods for mesh construction, iteration, and object creation/retrieval.

###### Members:

`std::vector<Traits::HType> m_halfEdges` – Storage of half-edges.

`std::vector<Traits::VType> m_vertices` – Storage of vertices.

`std::vector<Traits::EType> m_edges` – Storage of edges.

`std::vector<Traits::FType> m_faces` – Storage of faces.

###### Methods:

`HalfEdgeMesh()` – Default constructor.

`auto halfEdgeIterBegin()` / `halfEdgeIterEnd()` – Iterators over half-edges.

`auto vertexIterBegin()` / `vertexIterEnd()` – Iterators over vertices.

`auto edgeIterBegin()` / `edgeIterEnd()` – Iterators over edges.

`auto faceIterBegin()` / `faceIterEnd()` – Iterators over faces.

`static void getVerticesFromFace(FType* face, std::vector<VType>& vertices)` – Collects all vertices from a given face.

`template<typename T, typename Vector> T* createObject(Vector& storageVector)` – Creates a new object of type T and stores it.

`template<typename T, typename Key, typename Map> T* findObject(const Key& key, Map& helperMap)` – Finds an object by key in a helper map.

`template<typename T, typename Key, typename Map, typename Vector> T* findOrCreateObject(...)` – Finds an existing object or creates a new one if not found.

`void build(const std::vector<std::vector<int>>& polygons, const std::vector<glm::vec3>& vertices)` – Builds the half-edge mesh from polygon index data and vertex positions.
