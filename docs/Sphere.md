# Sphere
The Sphere struct represents a 3D sphere defined by its center position and radius. It includes a utility method to test whether a given point lies inside or on the surface of the sphere.

###### Public Methods
`bool containsPoint(const glm::vec3& point) const`
Checks if a given point is inside or on the surface of the sphere.
Returns true if the squared distance between the point and the sphere's center is less than or equal to the squared radius.