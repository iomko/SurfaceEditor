# BrushInteractionHandler
Handles user interactions specific to brush tools within the viewport. Maintains the state of the last known hit point and brush radius during the interaction lifecycle (onBegin, onUpdate, onEnd).

### BrushInteractionHandler class

######  Methods:
`BrushInteractionHandler(BrushToolCommand* command)` - Constructor initializing the handler with a pointer to a brush command.  

`void onBegin(const BrushToolParams& iParams)` - Called when a brush interaction begins. Executes the command with the initial parameters and stores the initial hit point and radius.  

`void onUpdate(const BrushToolParams& iParams)` - Called during brush interaction updates.  

`void onEnd(const BrushToolParams& iParams)` - Called when the brush interaction ends.  
