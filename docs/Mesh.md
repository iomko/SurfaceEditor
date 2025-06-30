#Mesh
The Mesh class represents a 3D polygon mesh object using a Half-Edge Data Structure for efficient geometry processing. It optionally associates a material and tracks selection status for editor or UI usage.

###### Public Methods

`Mesh(Material* defaultMaterial, std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices, const std::vector<int>& polygonNormalIndices, const std::vector<glm::vec3>& polygonsNormals = {})`
Creates a mesh from vertex data and polygon indices, optionally with normals and normal indices.

`Mesh(Material* defaultMaterial, std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices)`
Simplified constructor without normals. Calls build() on the half-edge structure.

`bool isBuildSuccessful()`
Returns the value of the m_buildSuccessful flag. Currently always false unless manually changed.

`HalfEdgeDS::HalfEdgeMesh* getHalfEdgeStructure()`
Returns the internal pointer to the half-edge mesh structure, allowing low-level access and manipulation.
