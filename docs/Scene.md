# Scene

### SceneResources
A utility struct that organizes and stores the spatial mapping between scene geometry (meshes and their faces) and corresponding octrees in the scene.

### SceneUtilities
A helper class providing static utility functions for octree-related coordinate and bounding box calculations.

###### Public Static Methods:
`std::pair<SceneResources::MeshFacePair, glm::vec3> retClosestHitData(Camera* camera, Window* window, SceneResources& res)`
Returns the closest intersected face in the scene under the camera's ray, along with the voxel coordinate in which the hit occurred.

`std::pair<glm::vec3, glm::vec3> calculateOctreeBounds(const glm::vec3& voxelIndex, const glm::vec3& voxelSize)`
Computes the world-space bounds (min, max) of an octree voxel based on its index and size.

`glm::vec3 calculateOctreeIDFromOctree(Octree<HalfEdgeDS::Face*>* octree, const glm::vec3& voxelSize)`
Returns the voxel index corresponding to a given octree based on its bounds and voxel size.

`glm::vec3 getVoxelIndex(const glm::vec3& bounds, const glm::vec3& voxelSize)`
Computes the voxel index in 3D space for a given position and voxel size.

### Scene
Manages the voxel-based spatial partitioning of scene geometry using octrees. Supports insertion and deletion of faces and meshes into/from octrees based on their bounding volumes.

###### Public Methods:

`Scene(float xSize, float ySize, float zSize)`
Initializes a Scene with a given voxel size in X, Y, and Z directions.

`~Scene()`
Default destructor.

`glm::vec3 getVoxelSize()`
Returns the current voxel size used in the scene.

`void addFaceIntoOctrees(Mesh* mesh, HalfEdgeDS::Face* face)`
Inserts a face into all the octrees corresponding to the voxels it intersects, creating new octrees if necessary. Tracks the voxel locations per face.

`void deleteMeshFromOctrees(Mesh* mesh)`
Removes all faces of a mesh from all octrees and cleans up empty octrees and references.

`void deleteFaceFromOctrees(Mesh* mesh, HalfEdgeDS::Face* face)`
Removes a single face from all octrees it occupies. If an octree becomes empty, it is removed from the scene.