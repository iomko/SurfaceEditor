# Tool

### ITool
Abstract interface class representing a generic tool that holds an interaction handler.

###### Public Methods:
`ITool(InteractionHandlerConcept* interactionHandler)`
Constructor that initializes the tool with a given interaction handler pointer.

`virtual ~ITool()`
Virtual destructor for safe polymorphic destruction.

`virtual InteractionHandlerConcept* getInteractionHandler()`
Returns a pointer to the associated interaction handler.

### Tool<CommandT, InteractionHandlerT>
Template class representing a concrete tool binding a specific command type with a specific interaction handler type.

Inherits from ITool.

###### Public Methods:
`Tool(CommandT* command)`
Constructor that creates an instance of the interaction handler InteractionHandlerT using the given command pointer and passes it to the base ITool constructor.