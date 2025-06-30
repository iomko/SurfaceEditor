# DeselectFaceCommand

The DeselectFaceCommand is a simple command used to deselect currently selected face(s) in the scene.
It follows the command pattern and inherits from Observable, allowing registered observers to react when the command is executed.

This command operates without any input parameters and acts on the current selection state.

###### Public Methods
`void execute()`
Executes the command by notifying all registered observers. Observers are expected to handle the logic for deselecting currently selected face(s).

`void undo()`
Currently not implemented. May be extended in the future to support undoing the deselection (e.g., reselecting the previously deselected face).

`static constexpr std::string_view getCommandName()`
Returns the unique name identifier of the command: "DeselectFaceCommand".

	