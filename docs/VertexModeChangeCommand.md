# VertexModeChangeCommand

The VertexModeChangeCommand class represents a command to change the vertex editing mode.
It inherits from Command<> with no parameters and Observable to notify observers when the mode change occurs.

###### Public Methods
`void execute()`
Notifies all observers that the vertex mode has changed.

`void undo()`
Currently unimplemented; intended for undoing the mode change if needed.

`static constexpr std::string_view getCommandName()`
Returns the command's unique identifier string: "VertexModeChangeCommand".
