# BrushToolCommand

The BrushToolCommand class represents a command that executes brush-based operations on a scene, typically for sculpting, painting, or terrain editing.
It inherits from Command<BrushToolParams, OctreeNodeDataParams> and Observable, allowing it to send both input and output parameters to observers.

###### Public Methods
`void execute(const BrushToolParams& iParams, OctreeNodeDataParams& oParams)`
Executes the brush tool operation by notifying all observers with the provided input parameters (iParams) and collecting results or updates in oParams.

`void undo()`
Currently not implemented. Reserved for reversing the effects of the brush operation in future implementations.

`static constexpr std::string_view getCommandName()`
Returns the internal string identifier "BrushToolCommand" used for logging, command registry, or debugging.