# AABB

### AABBBoundingRegion
AABBBoundingRegion
The AABBBoundingRegion class represents an Axis-Aligned Bounding Box (AABB) used for spatial queries in 3D space. It efficiently encapsulates a volume defined by two corner points (minimum and maximum) aligned with coordinate axes. This class provides methods to construct bounding boxes from point sets, test for intersections with rays, spheres, and other AABBs, and check containment of points or other bounding boxes.

###### Public Methods
`template <typename It, typename PointExtractor> AABBBoundingRegion(It begin, It end, PointExtractor extractor)`
Constructs an AABB that bounds all points obtained by applying extractor on elements in the range (begin, end).
The extractor should be a callable that returns a glm::vec3 representing the position of the element.

`AABBBoundingRegion(const glm::vec3& min, const glm::vec3& max)`
Constructs an AABB using explicit minimum and maximum corner coordinates. The constructor ensures that bounds[0] is the minimum corner and bounds[1] is the maximum corner.

`bool intersectsRay(const Ray& ray) const`
Checks if a given Ray intersects the AABB. Uses the "slab method" optimized with precomputed ray direction inverses and sign information for efficient intersection testing.

`bool intersectsSphere(const Sphere& sphere) const`
Tests whether the AABB intersects a given Sphere. The method clamps the sphere's center to the AABB and checks if the distance from the sphere center to the closest point in the AABB is less than or equal to the sphere radius.

`bool intersectsAABB(const AABBBoundingRegion& aabb) const`
Determines whether this AABB intersects with another AABB. Returns true if the two bounding boxes overlap along all three axes.

`bool containsPoint(const glm::vec3& point) const`
Returns true if the given 3D point lies inside or on the boundary of the AABB.

`bool containsAABB(const AABBBoundingRegion& aabb) const`
Returns true if the given AABB is completely contained within this AABB.

`const glm::vec3& getMinBoundsPos() const`
Returns the minimum corner point of the AABB.

`const glm::vec3& getMaxBoundsPos() const`
Returns the maximum corner point of the AABB.

`AABBBoundingRegion(const AABBBoundingRegion& other)`
Copy constructor.

`AABBBoundingRegion& operator=(const AABBBoundingRegion& other)`
Assignment operator.

`bool operator<(const AABBBoundingRegion& other) const`
Defines a strict ordering for AABBs based on their minimum and maximum corner coordinates. Useful for sorting or storing in ordered containers.

`bool operator==(const AABBBoundingRegion& other) const`
Equality comparison operator. Returns true if both bounding boxes have identical minimum and maximum corners.

`bool operator!=(const AABBBoundingRegion& other) const`
Inequality comparison operator.