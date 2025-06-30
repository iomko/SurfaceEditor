# Ray
Provides functionality for ray construction, camera-based ray generation, and geometric intersection tests (sphere, plane, triangle).

###### Public Methods:

`Ray()`
Default constructor creating an empty ray.

`Ray(const glm::vec3& origin, const glm::vec3& direction)`
Constructs a ray with a given origin and direction. Precomputes inverse direction and axis signs for fast bounding box intersection tests.

`Ray getCameraRay(Camera& camera)`
Returns a ray that originates from the camera's position and points in the direction the camera is facing.

`Ray fromMousePos(Camera& camera, Window& window)`
Generates a ray from the current mouse position on the screen, projected into world space based on the camera's view and projection matrices.

`bool intersectsSphere(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const Sphere& sphere)`
Tests if the ray defined by rayOrigin and rayDir intersects a given Sphere. Returns true if the ray intersects the sphere, false otherwise.

`bool intersectPlane(const glm::vec3& n, const glm::vec3& p0, const glm::vec3& l0, const glm::vec3& l, float& t)`
Tests for intersection between a ray and a plane.

`bool intersectsTriangle(const glm::vec3& orig, const glm::vec3& dir, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, float& t)`
Checks for intersection between a ray and a triangle using the Moller-Trumbore algorithm.
Returns true if the ray intersects the triangle and stores the distance t to the intersection point.
