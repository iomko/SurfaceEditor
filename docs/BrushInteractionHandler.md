# BrushInteractionHandler
Handles user interactions specific to brush tools within the viewport.
Extends InteractionHandler and manages the execution of a BrushToolCommand based on real-time user input and context.

Maintains the state of the last known hit point and brush radius during the interaction lifecycle (onBegin, onUpdate, onEnd).

###### Public Methods:
`BrushInteractionHandler(BrushToolCommand* command)`
Constructor initializing the handler with a pointer to a brush command.

`void onBegin(const BrushToolParams& iParams)`
Called when a brush interaction begins. Executes the command with the initial parameters and stores the initial hit point and radius.

`void onUpdate(const BrushToolParams& iParams)`
Called during brush interaction updates.
Determines the current hit point from the scene using ray casting. If the new hit point lies outside the current brush sphere, re-executes the command and updates internal state.

`void onEnd(const BrushToolParams& iParams)`
Called when the brush interaction ends. Outputs a message indicating completion.
