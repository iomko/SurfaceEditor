# Scene

### SceneResources class
A utility struct that organizes and stores mappings for scene geometries.  

### SceneUtilities class
A helper class providing utility functions for octree-related coordinate and bounding box calculations.  

###### Methods:
`std::pair<FaceRef, glm::vec3> SceneUtilities::retClosestHitData(Camera* camera, Window* window, Scene& scene)` - Queries the scene's spatial indices for candidate faces, performs exact ray-triangle tests, and returns the nearest hit as a mesh/face pair and world-space hit point. If nothing is hit, the pair contains null pointers and the hit point is zero.

`std::pair<glm::vec3, glm::vec3> calculateOctreeBounds(const glm::vec3& voxelIndex, const glm::vec3& voxelSize)` - Computes the world-space bounds (min, max) of an octree voxel based on its index and size.  

`glm::vec3 calculateOctreeIDFromOctree(Octree<HalfEdgeDS::Face*>* octree, const glm::vec3& voxelSize)` - Returns the voxel index corresponding to a given octree based on its bounds and voxel size.  

`glm::vec3 getVoxelIndex(const glm::vec3& bounds, const glm::vec3& voxelSize)` - Computes the voxel index in 3D space for a given position and voxel size.  

### Scene class
Manages the voxel-based spatial partitioning of the scene using octrees.  

`Scene` indexes meshes and faces for spatial queries; it does not own
`ModelObject` instances. Model objects and their attached meshes are owned by
the application-level [ModelObjectRepository](ModelObjectRepository.md).
Each mesh stores its own face spatial index; the scene stores the mesh index
and queries each candidate mesh's face index. See [ModelObject](ModelObject.md)
for the relationship between models and meshes. The `SceneTemplate` mesh
index must satisfy the [`SpatialIndex`](Scene.md#sym:SpatialIndex) concept.

#### Spatial-index types
[`SceneSpatialIndex.h`](../src/Scene/SceneSpatialIndex.h) is the central
selection point for the scene's spatial-index implementations. It declares
`MeshSpatialIndex` for indexing meshes and `FaceSpatialIndex` for indexing
faces. Both aliases currently use `OctreeSpatialIndex`.

`SceneTemplate` uses both aliases: `Scene` is
`SceneTemplate<MeshSpatialIndex>`, and the scene creates and queries face
indices using `FaceSpatialIndex`. Each `Mesh` stores its face index through
`std::unique_ptr<FaceSpatialIndex>`. Change the aliases in
`SceneSpatialIndex.h` to select different implementations without referring
to concrete index types in `Mesh.h` or in the `Scene` alias.

Both selected types must satisfy the [`SpatialIndex`](Scene.md#sym:SpatialIndex)
concept. The face index must also support the operations used by
`SceneTemplate`: insertion, removal, and ray, AABB, and sphere queries. The
single-configuration constructor can be used when both index types accept the
same configuration. Use the explicit mesh-index and face-index-factory
constructor when they need different parameters.

###### Methods:
`Scene{indexConfig}` - Initializes the mesh and face spatial indices from the same configuration object, when both index types can be constructed from it. For the octree-backed `Scene`, this is a `glm::vec3` voxel size, for example `Scene scene{glm::vec3{25.0f}}`.  
`Scene(meshIndex, faceIndexFactory)` - Initializes the scene with an explicitly constructed mesh spatial index and a factory that creates face spatial indices. Use this form when the two index types need different constructor parameters or custom construction.  
`glm::vec3 getVoxelSize()` - Returns the current voxel size used in the scene.  
`void addFaceIntoOctrees(Mesh* mesh, HalfEdgeDS::Face* face)` - Inserts a face into all the octrees corresponding to the voxels it intersects, creating new octrees if necessary.  
`void deleteMeshFromOctrees(Mesh* mesh)` - Removes all faces of a mesh from all octrees.  
`void deleteFaceFromOctrees(Mesh* mesh, HalfEdgeDS::Face* face)` - Removes a single face from all octrees it occupies.  
