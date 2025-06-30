# FaceDeselectionTool
Tool for deselecting faces in a scene.

Inherits from Tool, specialized with DeselectFaceCommand and DeselectionHandler<DeselectFaceCommand> to handle deselection interactions.

###### Public Methods:
`FaceDeselectionTool(DeselectFaceCommand* command)`
Constructor that initializes the tool with a pointer to a DeselectFaceCommand.

`static std::string getToolName()`
Returns the name of the tool as a string: "FaceDeselectionTool".
