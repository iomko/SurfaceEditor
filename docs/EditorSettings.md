# EditorSettings

### CameraSettings
The CameraSettings struct provides global configuration values that control camera movement and rotation responsiveness in the application. 
These settings are used by camera controllers to interpret user input (such as keyboard or mouse movement) and apply appropriate motion or rotation.

###### Members
  - `static float m_movementSensitivity`  
    Determines how fast the camera moves in response to user input. Higher values make the camera move faster.

  - `static float m_rotationSensitivity`  
    Controls how sensitive the camera is to mouse movement when rotating. Higher values result in quicker rotations.
