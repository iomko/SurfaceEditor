# GeometryUtils

### utils::geometry
Namespace providing utility functions for geometric computations.

###### Methods:
`std::vector<glm::vec3> triangulatePolygon(const std::vector<glm::vec3>& vertices)`
Triangulates a polygon defined by coplanar 3D vertices using constrained Delaunay triangulation.

`std::vector<glm::vec2> projectVertices(const std::vector<glm::vec3>& vertices, const ProjectionAxis& projectionAxis)`
Projects 3D vertices onto a 2D plane by dropping one coordinate axis based on the selected projection axis (ZY, ZX, or XY).

`template <typename Iterator, typename PointExtractor> glm::vec3 computePolygonNormal(Iterator begin, Iterator end, PointExtractor extractor)`
Computes the normal vector of a polygon defined by a range of vertices extracted via the provided point extractor. Uses direct cross product for triangles and Newell's method for polygons with more than three vertices.

`glm::vec3 computePolygonNormal(HalfEdgeDS::Face* face)`
Computes the polygon normal of a face in a half-edge data structure by extracting vertex positions.

`float computePolygonArea(const std::vector<glm::vec2>& vertices)`
Calculates the signed area of a 2D polygon using the shoelace formula. Returns 0 if fewer than 3 vertices.

`Plane fitPlaneToPoints(const std::vector<glm::vec3>& vertices, bool& allCoplanar)`
Fits a plane to a set of points by using the first three points to define the plane normal and checking coplanarity of remaining points within an epsilon tolerance. Sets allCoplanar to true if all points lie on the plane.

`glm::vec3 intersectRayPlane(const Plane& plane, const glm::vec3& origin, const glm::vec3& direction, bool& hit)`
Calculates the intersection point of a ray (origin + t * direction) with a plane. Sets hit to true if intersection exists and lies in front of the ray origin.

### utils::geometry::detail
Namespace containing internal helper functions for geometry operations.

###### Methods:
`std::vector<tpp::Delaunay::Point> toDelaunayPoints(const std::vector<glm::vec2>& vertices)`
Converts a vector of 2D glm::vec2 vertices to a vector of tpp::Delaunay::Point suitable for triangulation input.

`tpp::Delaunay createDelaunay(const std::vector<tpp::Delaunay::Point>& vertices)`
Creates a constrained Delaunay triangulation object from a set of vertices by adding edges as segment constraints between consecutive vertices and performing triangulation.

`glm::vec3 tryProjectOnPlane(const Plane& plane, const glm::vec3& vertex, const glm::vec3& vertexNormal)`
Attempts to project a vertex onto a given plane along the direction of vertexNormal. If this fails (no intersection), it attempts projection along the opposite direction.

`glm::vec3 liftTo3D(const ProjectionAxis& projectionAxis, const tpp::Delaunay::Point& vertex)`
Lifts a 2D triangulation point back into 3D space by assigning the missing coordinate based on the chosen projection axis (ZY, ZX, or XY), placing the lifted point on the corresponding 3D plane.