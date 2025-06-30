# AddPlaneCommand

The AddPlaneCommand class represents a command in the command pattern responsible for adding a new plane to the scene or editor.
It inherits from Command<PlaneParams> and Observable, allowing it to notify registered observers with the given parameters.

###### Public Methods
`void execute(const PlaneParams& params)`
Executes the command by notifying all registered observers with the specified PlaneParams.
Typically used to trigger updates in the scene or UI when a new plane is added.

`void undo()`
Currently unimplemented. Intended to reverse the plane addition in future versions.

`static constexpr std::string_view getCommandName()`
Returns the string identifier "AddPlaneCommand" for internal command management or debugging purposes.