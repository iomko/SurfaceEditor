# KeyEvents

### KeyEvent
`KeyEvent` is a general-purpose base class for all keyboard-related events.  
It stores the key code associated with the keyboard event.

###### Public Methods

- `KeyEvent(int keyCode)`  
  Creates a `KeyEvent` with the provided keyboard key code.

---

### Derived classes of `KeyEvent` class:

### KeyPressEvent  
  `KeyPressEvent` represents the triggered event when a key on the keyboard is pressed.

###### Public Methods
  - `KeyPressEvent(int keyCode)`  
    Creates a `KeyPressEvent` event using the given key code.

  - `virtual EventType getType() override`  
    Returns `EventType::KeyPress` to identify the type of the event.

  - `static EventType getStaticType()`  
    Returns the static type `EventType::KeyPress`.

  - `virtual const char* getName() override`  
    Returns the name of the event, `"keyPressEvent"`.

### KeyReleaseEvent  
  `KeyReleaseEvent` represents the triggered event when a key on the keyboard is released.

###### Public Methods
  - `KeyReleaseEvent(int keyCode)`  
    Creates a `KeyReleaseEvent` using the given key code.

  - `virtual EventType getType() override`  
    Returns `EventType::KeyRelease` to identify the type of the event.

  - `static EventType getStaticType()`  
    Returns the static type `EventType::KeyRelease`.

  - `virtual const char* getName() override`  
    Returns the name of the event, `"keyReleaseEvent"`.

---
	
### MouseScrollEvent
`MouseScrollEvent` represents the triggered event when the mouse wheel is scrolled.

###### Public Methods
- `MouseScrollEvent(double xOffset, double yOffset)`  
  Creates a `MouseScrollEvent` with the horizontal (xOffset) and vertical (yOffset) scroll amounts, which represents how much the user scrolled using a mouse wheel or trackpad.

- `virtual EventType getType() override`  
  Returns `EventType::MouseScroll` to identify the type of the event.

- `static EventType getStaticType()`  
  Returns the static type `EventType::MouseScroll`.

- `virtual const char* getName() override`  
  Returns the name of the event, `"mouseScrollEvent"`.

---

### MouseMoveEvent  
`MouseMoveEvent` is triggered when the mouse is moved.

###### Public Methods
- `MouseMoveEvent(double xPos, double yPos)`  
  Creates a `MouseMoveEvent` with the given X and Y positions of the mouse.

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
  Creates a `MouseButtonPressEvent` using the given mouse button code.

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
  Creates a `MouseButtonReleaseEvent` using the given mouse button code.

- `virtual EventType getType() override`  
  Returns `EventType::MouseButtonRelease` to identify the type of the event.

- `static EventType getStaticType()`  
  Returns the static type `EventType::MouseButtonRelease`.

- `virtual const char* getName() override`  
  Returns the name of the event, `"mouseButtonReleaseEvent"`.
