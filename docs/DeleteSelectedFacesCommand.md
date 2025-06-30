# DeleteSelectedFacesCommand

The DeleteSelectedFacesCommand represents a command used to trigger deletion of currently selected faces within the scene.
It follows the command pattern and is observable, allowing external systems (e.g., rendering or UI) to react when the command is executed.

This class does not take any parameters and operates on existing selection state, delegating actual deletion logic to observers.

###### Public Methods
`void execute()`
Executes the command by notifying all observers. The observers are expected to handle deletion of the selected faces.

`void undo()`
This method is currently not implemented. It could be extended to support undoing the deletion operation in the future (e.g., by storing backup of deleted faces).

`static constexpr std::string_view getCommandName()`
Returns the unique string identifier of the command: "DeleteSelectedFacesCommand".
	