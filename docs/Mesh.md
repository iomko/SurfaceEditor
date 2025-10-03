#   Mesh

### Mesh class
The Mesh class represents a 3D polygonal mesh object that is built up on Half-Edge Data Structure for efficient geometry processing.  

######  Methods
`Mesh(Material* defaultMaterial, std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices, const std::vector<int>& polygonNormalIndices, const std::vector<glm::vec3>& polygonsNormals = {})` - Creates a mesh from vertex data and polygon indices, optionally with normals and normal indices.  

`Mesh(Material* defaultMaterial, std::vector<std::vector<int>>& polygonsIndices, const std::vector<glm::vec3>& polygonsVertices)` - Simplified constructor without normals.  

`bool isBuildSuccessful()` - Returns the value of the m_buildSuccessful flag. Currently not implemented.  

`HalfEdgeDS::HalfEdgeMesh* getHalfEdgeStructure()` - Returns the internal pointer to the half-edge mesh structure.  
