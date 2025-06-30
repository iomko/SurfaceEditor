# TransformMeshCommand

The TransformMeshCommand class handles the transformation of a mesh using given parameters.
It inherits from Command<TransformMeshParams> to accept transformation parameters and Observable to notify observers when the command executes.

When executed, it sends the transformation parameters to all registered observers.

###### Public Methods
`void execute(const TransformMeshParams& params)`
Notifies observers with the provided transformation parameters to apply mesh transformations.

`void undo()`
Currently unimplemented; intended for future undo functionality.

`static constexpr std::string_view getCommandName()`
Returns the unique command identifier: "TransformMeshCommand".