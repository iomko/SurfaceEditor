# MeshSelectionTool
Class representing a tool for selecting mesh objects.

Inherits from the generic Tool class, using the command SelectMeshCommand and the interaction handler SelectionHandler<SelectMeshCommand>.

###### Public Methods:
`MeshSelectionTool(SelectMeshCommand* command)`
Constructor that initializes the base Tool class with the given command.

`static std::string getToolName()`
Returns the tool's name: "MeshSelectionTool".