# SelectFaceCommand

The SelectFaceCommand represents an action to select a face within the scene.
It inherits from Command<> (no parameters) and from Observable to notify observers when executed.

This command signals interested observers that a face selection has occurred.

###### Public Methods
`void execute()`
Notifies all observers about the face selection event.

`void undo()`
Undo logic is currently empty and can be implemented as needed.

`static constexpr std::string_view getCommandName()`
Returns the command's unique name: "SelectFaceCommand".
