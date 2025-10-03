# Input

### Input class
The `Input` class provides methods to handle user input, including keyboard and mouse interaction.

###### Methods

`static void updateButtonClicks()` - Resets the state of all mouse button clicks. It is being called at the end of each frame.  

`static void updateMousePosition()` - Updates the stored mouse position to the current cursor position.  
  
`static float getMouseDx()` - Returns the distance of a horizontal mouse movement since the last update.  
  
`static float getMouseDy()` - Returns the distance of a vertical mouse movement since the last update.  

`static bool isKeyPressed(int keycode)` - Returns true if the key was currently pressed.  

`static bool isKeyDown(int keycode)` - Returns true if the key is currently held down.  

`static bool isMouseButtonPressed(int button)` - Returns true if the mouse button was currently pressed.  

`static bool isMouseButtonDown(int mouse)` - Returns true if the mouse button is currently held down.  

`static float getMouseX()` - Returns the current X position of the mouse cursor.  

`static float getMouseY()` - Returns the current Y position of the mouse cursor.  


