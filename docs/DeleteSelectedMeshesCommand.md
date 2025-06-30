# DeleteSelectedMeshesCommand

The DeleteSelectedMeshesCommand is a command used to delete all currently selected meshes in the scene.
It implements the command pattern and inherits from Observable, allowing other parts of the application (e.g. scene manager, UI) to respond when the command is executed.

This command takes no parameters and acts based on the current mesh selection.

###### Public Methods
`void execute()`
Executes the command by notifying all registered observers. Observers are expected to implement the logic for removing selected meshes from the scene.

`void undo()`
Currently not implemented. It may be extended to support undo functionality (e.g., restoring deleted meshes from a backup).

`static constexpr std::string_view getCommandName()`
Returns the string identifier of the command: "DeleteSelectedMeshesCommand".
	