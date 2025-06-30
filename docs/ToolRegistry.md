# ToolRegistry
A static registry managing tool instances by their names. Allows registering, retrieving, and deleting tools.

###### Public Static Methods:
`template<typename ToolT, typename... Args> static void registerTool(Args&&... args)`
Registers a tool of type ToolT by forwarding constructor arguments. Stores the tool instance internally keyed by its static getToolName() string.

`template<typename ToolT> static ToolT* getTool()`
Retrieves a pointer to a registered tool of type ToolT. Returns nullptr if no such tool is registered.

`static void deleteRegistry()`
Deletes all registered tool instances and clears the registry.