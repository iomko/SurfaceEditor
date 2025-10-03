# Tool

### Tool<CommandT, InteractionHandlerT> class
Template class that represents a tool that has been binded with a command.  

######  Methods:
`Tool(CommandT* command)` - Constructor that creates an instance of the interaction handler using the given command pointer.  

#### All the Tool class instances used in the project:

`BrushTool` – Tool that deforms surface by applying a default brush.  

`FaceDeselectionTool` – Tool that deselects face.  

`FaceSelectionTool` – Tool that selects face.  

`MeshSelectionTool` – Tool that selects mesh.  

### ToolRegistry class
Class that stores all registered tools.  

`template<typename ToolT, typename... Args> static void registerTool(Args&&... args)` - Registers tool that will be added to a map.  
`template<typename ToolT> static ToolT* getTool()` - Retrieve tool from registry.  
`static void deleteRegistry()` - Deletes all the registered tools.  
