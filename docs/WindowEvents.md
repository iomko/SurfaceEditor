# WindowEvents

### WindowCloseEvent
`WindowCloseEvent` represents the triggered event when the application window is requested to close (e.g., when the user clicks the close button on the window).

###### Public Methods

- `WindowCloseEvent()`  
Creates a `WindowCloseEvent`. No additional parameters are required.
  
- `virtual EventType getType() override`  
Returns `EventType::WindowClose` to identify the type of the event.
  
- `static EventType getStaticType()`  
Returns the static type `EventType::WindowClose`.

- `virtual const char* getName() override`  
Returns the name of the event, `"windowCloseEvent"`.

---

### WindowResizeEvent
`WindowResizeEvent` represents the event triggered when the application window is resized.

###### Public Methods
  - `WindowResizeEvent(int width, int height)`  
    Creates a `WindowResizeEvent` with the new dimensions of the window specified by `width` and `height`.

  - `vvirtual EventType getType() override`  
    Returns `EventType::WindowResize` to identify the type of the event.

  - `static EventType getStaticType()`  
    Returns the static type `EventType::WindowResize`.

  - `virtual const char* getName() override`  
    Returns the name of the event, `"windowResizeEvent"`.