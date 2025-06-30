# OperationParams

This file defines a set of structs derived from the base OpParams, which act as parameter containers for various operations throughout the application. These are typically used to pass structured data to systems such as scene manipulation, tool interactions, and import/export logic.

All parameter structs inherit from OpParams, allowing for polymorphic use in generic operation handling.

### OpParams
Base struct used for polymorphic operation parameters. It defines a virtual destructor to ensure proper cleanup of derived classes.

### SelectionLayerParams
Defines parameters related to selection operations in the scene, such as selecting or deselecting objects or geometry.

SelectionMode can be one of: Face, Edge, Vertex, or Object.

Type specifies whether the operation is a selection or deselection.

Stores the current selection mode and operation type.

### TransformMeshParams
Encapsulates transformation data to be applied to a mesh.

Contains a model matrix (glm::mat4) representing the transformation.

Stores a pointer to the mesh that will be transformed.

### ImportExportMeshesParams
Stores the file path to use when importing or exporting mesh data.

The path is stored as a string and may be used by file dialogs or serialization systems.

### OpenTopoParams
Holds parameters required to query OpenTopo data (likely elevation or geospatial data).

Includes latitude and longitude bounds (lower left and upper right corners).

Stores an API key used for authentication.

### PlaneParams
Parameters used for procedural plane generation.

m_size defines the physical size of the generated plane.

m_subdivisionLevel controls how finely the plane is subdivided into smaller segments.

### SelectFaceParamsOut
Used to return data after a face selection operation has been processed.

Contains a pointer to the mesh and the specific face that was hit.

Stores the hit point in world space where the selection occurred.

### SelectFaceParamsIn
Contains input data for a face selection operation.

Stores a ray that is cast into the scene to determine which face (if any) is intersected.

### OctreeNodeDataParams
Parameters passed during octree interaction or queries.

Includes a MeshFacePair object (from SceneResources) and a hit point in 3D space.

### MeshParams
A generic container for operations that act on a single mesh.

Holds a pointer to the affected Mesh object.

### BrushToolParams
Stores parameters for a brush tool used in sculpting or painting operations.

Includes radius for the brush size and brushStrength for intensity.

### BrushInteractionParams
Combines brush tool parameters with contextual data required during interaction.

Stores brush parameters, as well as pointers to the Camera and Window to allow for proper projection and input handling.