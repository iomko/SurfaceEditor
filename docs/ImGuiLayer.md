# ImGuiLayer

Handles the initialization, rendering, and shutdown of the ImGui library integrated with GLFW and OpenGL.

Provides lifecycle methods to manage ImGui context and frame rendering within the application.

###### Public Methods:
`void onAttach()`
Initializes the ImGui context and configures IO flags for keyboard navigation, docking, and multi-viewport support.
Sets up the ImGui style and initializes platform/renderer backends for GLFW and OpenGL.

`void onDetach()`
Shuts down the ImGui platform/renderer backends and destroys the ImGui context.

`void begin()`
Prepares a new ImGui frame by calling the necessary backend new frame functions and starting a new ImGui frame.

`void end()`
Finalizes the ImGui frame by rendering the draw data, updating display size from the application window, and handling multiple viewports if enabled by updating and rendering platform windows accordingly.