# Window

### Window class
The Window class encapsulates the creation and management of a GLFW-based window along with an OpenGL rendering context.
It handles window initialization, forwards input events (like keyboard, mouse, and window events) to the `Application` through a user-defined callback. It also manages buffer swapping throughout the application's lifecycle.

###### Methods

`Window(int width, int height, const std::string& title)` - Constructs the window with the given dimensions and title. The window is not initialized until `initialize()` method is called.  
  
`void terminate()` - Properly shuts down GLFW and cleans up window resources.  
  
`void clearColorBuffer(float r, float g, float b, float a)` - Clears the color buffer with the specified RGBA color values.  

`void clearDepthBuffer()` - Clears the depth buffer, which resets all stored depth information.  

`void clearStencilBuffer()` - Clears the stencil buffer.  

`void clearAllBuffers(float r, float g, float b, float a)` - Clears color, depth, and stencil buffers.  

`void update()` - Swaps the front and back buffers to display new rendered frame. It also polls input events such as keyboard, mouse, or window events.  

`bool initialize()` - Initializes GLFW, creates the window and OpenGL context. It also sets up all the internal input callbacks. Returns true if the initialization succeeded.  

`std::string getScreenTitle()` - Returns the title of the window.  

`int getScreenWidth()` - Returns the current window width.  

`int getScreenHeight()` - Returns the current window height.  

`GLFWwindow* getWindowHandle()` - Returns a pointer to the underlying GLFW window handle.  

`void setEventFunc(std::function<void(Event&)> func)` - Sets the callback function that will be invoked when an event (like key press or window resize) occurs.  
