# MouseEvents

### MouseScrollEvent
`MouseScrollEvent` represents the triggered event when the mouse wheel or trackpad is scrolled.

###### Public Methods

- `MouseScrollEvent(double xOffset, double yOffset)`  
  Creates a `MouseScrollEvent` with the horizontal (`xOffset`) and vertical (`yOffset`) scroll amounts, indicating how far the user scrolled in each direction.
  
- `virtual EventType getType() override`  
Returns EventType::MouseScroll to identify the type of the event.
  
- `static EventType getStaticType()`  
Returns the static type EventType::MouseScroll.

- `virtual const char* getName() override`  
Returns the name of the event, "mouseScrollEvent".

---

### MouseMoveEvent
`MouseMoveEvent` represents the triggered event when the mouse cursor moves.

###### Public Methods
  - `MouseMoveEvent(double xPos, double yPos)`  
    Creates a `MouseMoveEvent` representing the new position of the mouse cursor at the given X (`xPos`) and Y (`yPos`) coordinates within the application window.

  - `virtual EventType getType() override`  
    Returns `EventType::MouseMove` to identify the type of the event.

  - `static EventType getStaticType()`  
    Returns the static type `EventType::MouseMove`.

  - `virtual const char* getName() override`  
    Returns the name of the event, `"mouseMoveEvent"`.
	
---

### MouseButtonPressEvent
`MouseButtonPressEvent` is triggered when a mouse button is pressed.

###### Public Methods
  - `MouseButtonPressEvent(int buttonCode)`  
    Creates a `MouseButtonPressEvent` using the given mouse button code, which specifies which button was pressed.

  - `virtual EventType getType() override`  
    Returns `EventType::MouseButtonPress` to identify the type of the event.

  - `static EventType getStaticType()`  
    Returns the static type `EventType::MouseButtonPress`.

  - `virtual const char* getName() override`  
    Returns the name of the event, `"mouseButtonPressEvent"`.

---

### MouseButtonReleaseEvent  
  `MouseButtonReleaseEvent` is triggered when a mouse button is released.

###### Public Methods
  - `MouseButtonReleaseEvent(int buttonCode)`  
    Creates a `MouseButtonReleaseEvent` using the given mouse button code, which specifies which button was released.

  - `virtual EventType getType() override`  
    Returns `EventType::MouseButtonRelease` to identify the type of the event.

  - `static EventType getStaticType()`  
    Returns the static type `EventType::MouseButtonRelease`.

  - `virtual const char* getName() override`  
    Returns the name of the event, `"mouseButtonReleaseEvent"`.