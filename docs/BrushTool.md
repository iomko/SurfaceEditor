# BrushTool
Represents a specific interactive tool for brush-based operations in the application.
Inherits from Tool, pairing a BrushToolCommand with a BrushInteractionHandler.

Used to encapsulate the logic, behavior, and name of the brush tool as a high-level interface.

###### Public Methods:
`BrushTool(BrushToolCommand* command)`
Constructor that initializes the tool with a pointer to its corresponding brush command.

`static std::string getToolName()`
Returns the name of the tool as a string literal: "BrushTool".
