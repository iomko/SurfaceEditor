## Event

The Event class defines various types of events that are processed by the LayerStack and propagated down through the layers. Each instance holds information about whether it has been handled or not.

###### Methods
`virtual EventType getType() = 0` - Returns the type of the event.  

`virtual const char* getName() = 0` - Returns the name of the event.  

These are the types of events used in the project:

`MouseMove`  
`MouseButtonPress`  
`MouseButtonRelease`  
`MouseScroll`  
`KeyPress`  
`KeyRelease`  
`WindowResize`  
`WindowClose`  
