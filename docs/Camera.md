# Camera

### Camera class
The Camera class represents a 3D camera that supports movement and rotation via mouse input.

######  Methods
`Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)` - Constructs the camera based on initial position, target point, and global up vector.  
`void updateCameraDirection(double diffMousePositionX, double diffMousePositionY)` - Updates the camera's orientation based on mouse movement deltas.  
`void updateCameraPosition(CameraMovement movementDirection)` - Moves the camera in the scene depending on the specified direction (FORWARD, BACKWARD, LEFT, RIGHT) using the defined movement sensitivity.  
`CameraState getState()` - Returns the internal CameraState.  
