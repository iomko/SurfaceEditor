# AABB

### AABBBoundingRegion class
The AABBBoundingRegion class represents an Axis-Aligned Bounding Box (AABB).  

###### Public Methods
`template <typename It, typename PointExtractor> AABBBoundingRegion(It begin, It end, PointExtractor extractor)` - Constructs an AABB that bounds all points obtained by applying extractor on elements in the range (begin, end).  
`AABBBoundingRegion(const glm::vec3& min, const glm::vec3& max)` - Constructs an AABB using explicit minimum and maximum corner coordinates.  
`bool intersectsRay(const Ray& ray) const` - Checks if a given Ray intersects the AABB.  
`bool intersectsSphere(const Sphere& sphere) const` - Tests whether the AABB intersects a given Sphere.  
`bool intersectsAABB(const AABBBoundingRegion& aabb) const` - Determines whether this AABB intersects with another AABB.  
`bool containsPoint(const glm::vec3& point) const` - Returns true if the given 3D point lies inside or on the boundary of the AABB.  
`bool containsAABB(const AABBBoundingRegion& aabb) const` - Returns true if the given AABB is completely contained within this AABB.  
`const glm::vec3& getMinBoundsPos() const` - Returns the minimum corner point of the AABB.  
`const glm::vec3& getMaxBoundsPos() const` - Returns the maximum corner point of the AABB.  
`bool operator<(const AABBBoundingRegion& other) const` - Defines a strict ordering for AABBs based on their minimum and maximum corner coordinates.  
`bool operator==(const AABBBoundingRegion& other) const` - Equality comparison operator. Returns true if both bounding boxes have identical minimum and maximum corners.  
`bool operator!=(const AABBBoundingRegion& other) const` - Inequality comparison operator.  
