# SelectVertexCommand

The SelectVertexCommand class represents the action of selecting a vertex within the scene.
It inherits from Command<> (no parameters) and Observable, enabling observer notification upon execution.

When executed, it informs all registered observers that a vertex selection has occurred.

###### Public Methods
`void execute()`
Notifies observers that a vertex has been selected.

`void undo()`
Currently unimplemented; can be extended to support undo functionality.

`static constexpr std::string_view getCommandName()`
Returns the unique command identifier: "SelectVertexCommand".