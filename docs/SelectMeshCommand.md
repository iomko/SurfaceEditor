# SelectMeshCommand

The SelectMeshCommand class represents an action to select a mesh in the scene.
It inherits from Command<> (no parameters) and Observable to notify observers when the command executes.

When executed, this command notifies all registered observers about the mesh selection event.

###### Public Methods
`void execute()`
Triggers notification to observers indicating a mesh has been selected.

`void undo()`
Undo behavior is not implemented but can be added if needed.

`static constexpr std::string_view getCommandName()`
Returns the unique command name: "SelectMeshCommand".