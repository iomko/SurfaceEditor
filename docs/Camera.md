# Camera

The Camera class represents a 3D camera system that supports movement and rotation via mouse input.
It uses glm for vector and matrix math, including directional vector calculation and view matrix generation using glm::lookAt.
It supports basic movement (forward, backward, left, right) and pitch/yaw-based rotation.

###### Public Methods
`Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)`
Constructs the camera based on initial position, target point, and global up vector.
Computes the initial front, right, and up vectors, and generates the view matrix using glm::lookAt.

`void updateCameraDirection(double diffMousePositionX, double diffMousePositionY)`
Updates the camera's orientation based on mouse movement deltas.
Modifies yaw and pitch values and clamps pitch to [-89 degrees, 89 degrees] to prevent gimbal lock.
Recalculates the front vector accordingly and updates all direction vectors.

`void updateCameraPosition(CameraMovement movementDirection)`
Moves the camera in the scene depending on the specified direction (FORWARD, BACKWARD, LEFT, RIGHT)
using the defined movement sensitivity. Also updates the view (lookAt) matrix.

`CameraState getState()`
Returns a copy of the internal CameraState, which contains the current position, direction vectors, and view matrix.
