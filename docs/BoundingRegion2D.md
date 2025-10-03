# BoundingRegion2D

### BoundingRegion2D class
The BoundingRegion2D class represents a two-dimensional axis-aligned bounding box (AABB) defined by minimum and maximum corner coordinates in 2D space.  

###### Methods:
template <typename It, typename PointExtractor> BoundingRegion2D(It begin, It end, PointExtractor extractor) - Constructs a 2D bounding region that tightly encloses all points in the given iterator range [begin, end), using the provided extractor function.  
BoundingRegion2D(const glm::vec2& min, const glm::vec2& max) - Constructs a 2D bounding region from explicit minimum and maximum corner coordinates.  
bool intersectsBoundingRegion2D(const BoundingRegion2D& boundingRegion2D) const - Determines whether the bounding region intersects with another BoundingRegion2D.  
bool intersectsRay(const Ray2D& ray) const - Checks if a given 2D ray intersects the bounding region.  
const glm::vec2& getMinBoundsPos() const - Returns the minimum corner point of the bounding region.  
const glm::vec2& getMaxBoundsPos() const - Returns the maximum corner point of the bounding region.  
bool operator<(const BoundingRegion2D& other) const - Defines a strict ordering for bounding regions based on their min and max corner coordinates.  
bool operator==(const BoundingRegion2D& other) const - Equality comparison operator. Returns true if both bounding regions have identical minimum and maximum corners.  
bool operator!=(const BoundingRegion2D& other) const - Inequality comparison operator. Returns true if this bounding region differs from other.  
