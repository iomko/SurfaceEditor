# DeselectMeshCommand

The DeselectMeshCommand is a simple command that triggers the deselection of currently selected mesh(es) in the scene.
It inherits from Observable and uses the command pattern to notify any registered observers when executed.

The command does not take any input parameters and delegates the deselection logic to its observers.

###### Public Methods
`void execute()`
Notifies all observers that a mesh deselection should occur. Observers implement the actual deselection logic.

`void undo()`
Currently not implemented. Can be extended in the future to support undo operations (e.g., reselecting previously deselected mesh).

`static constexpr std::string_view getCommandName()`
Returns the unique command identifier: "DeselectMeshCommand".