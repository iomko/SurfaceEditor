# Renderer

The Renderer class manages the setup and drawing of various rendering primitives using OpenGL.
It handles buffer creation, data uploading, and drawing calls for points, lines, meshes, and axis-aligned bounding boxes (AABBs).
Buffers are registered and initialized once, and then updated dynamically for each draw call.

###### Public Methods
`static void init()`
Initializes OpenGL buffers and vertex array objects (VAOs) for different primitive types: points, lines, meshes, and AABBs.
It sets up buffer layouts and allocates GPU memory with appropriate usage hints (e.g., GL_DYNAMIC_DRAW).

`static void drawPoints(std::vector<RendererStageData::PointVertex>& points)`
Uploads and draws a collection of points.
Points are rendered as GL_POINTS with a fixed size.

`static void drawLines(std::vector<RendererStageData::LineVertex>& lines)`
Uploads and draws a collection of lines.
Lines are rendered as GL_LINES with a specified line width.

`static void drawMesh(const std::vector<RendererStageData::MeshVertex>& mesh)`
Uploads and draws a mesh represented as triangles.
Uses vertex positions, normals, and highlight flags.

`static void drawBox(const std::vector<RendererStageData::AABBVertex>& box)`
Uploads and draws axis-aligned bounding boxes (AABBs).
Draws wireframe boxes by switching polygon mode to line.

###### Internal Types and Data
`RendererStageData`
Holds vertex data structures and maps associating meshes/materials/AABBs with vertex lists.

`BufferRegistry`
Manages OpenGL buffers (VAOs, VBOs, optionally EBOs) for different buffer types (Point, Line, Mesh, AABB).
Buffers are registered once and queried on demand.