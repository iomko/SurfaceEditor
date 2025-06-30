# FaceSelectionTool
Tool for selecting faces in a scene.

Inherits from Tool, specialized with SelectFaceCommand and SelectionHandler<SelectFaceCommand> to handle selection interactions.

###### Public Methods:
`FaceSelectionTool(SelectFaceCommand* command)`
Constructor that initializes the tool with a pointer to a SelectFaceCommand.

`static std::string getToolName()`
Returns the name of the tool as a string: "FaceSelectionTool".