### InteractionHandlerConcept
Abstract base class defining the interface for interaction handlers.

Declares pure virtual methods for handling the start, update, and end phases of an interaction, both with and without input parameters. Also provides a virtual method to retrieve the associated command.

###### Public Methods:
`virtual ~InteractionHandlerConcept()`
Virtual destructor.

`virtual void onBegin()`
Pure virtual method called at the beginning of an interaction without parameters.

`virtual void onUpdate()`
Pure virtual method called during interaction updates without parameters.

`virtual void onEnd()`
Pure virtual method called at the end of an interaction without parameters.

`virtual void onBegin(const OpParams& iParams)`
Pure virtual method called at the beginning of an interaction with parameters.

`virtual void onUpdate(const OpParams& iParams)`
Pure virtual method called during interaction updates with parameters.

`virtual void onEnd(const OpParams& iParams)`
Pure virtual method called at the end of an interaction with parameters.

`virtual CommandConcept* getCommand() const`
Returns a pointer to the associated command (default returns nullptr).

### InteractionHandler<CommandT, IParams = OpParams>
Template class providing a base implementation of InteractionHandlerConcept for commands of type CommandT and input parameters of type IParams (defaulted to OpParams).

Specializes behavior for handling interactions either with or without parameters depending on the template parameters.

Specialization: 
### InteractionHandler<CommandT, OpParams>
Handles interactions without input parameters.

###### Public Methods:
`InteractionHandler(CommandT* command)`
Constructor storing a pointer to the command.

`virtual void onBegin()`
Pure virtual method for beginning interaction without parameters.

`virtual void onUpdate()`
Pure virtual method for updating interaction without parameters.

`virtual void onEnd()`
Pure virtual method for ending interaction without parameters.

`void onBegin(const OpParams&)`
Throws logic_error: "onBegin with params not supported."

`void onUpdate(const OpParams&)`
Throws logic_error: "onUpdate with params not supported."

`void onEnd(const OpParams&)`
Throws logic_error: "onEnd with params not supported."

`CommandT* getCommand() const`
Returns the stored command pointer.

General Template: 
### InteractionHandler<CommandT, IParams> (IParams != OpParams)
Handles interactions with input parameters of type IParams.

###### Public Methods:
`InteractionHandler(CommandT* command)`
Constructor storing a pointer to the command.

`virtual void onBegin(const IParams&)`
Pure virtual method for beginning interaction with parameters.

`virtual void onUpdate(const IParams&)`
Pure virtual method for updating interaction with parameters.

`virtual void onEnd(const IParams&)`
Pure virtual method for ending interaction with parameters.

`void onBegin(const OpParams& iParams)`
Calls onBegin with IParams cast from OpParams.

`void onUpdate(const OpParams& iParams)`
Calls onUpdate with IParams cast from OpParams.

`void onEnd(const OpParams& iParams)`
Calls onEnd with IParams cast from OpParams.

`void onBegin()`
Throws logic_error: "onBegin without params not supported."

`void onUpdate()`
Throws logic_error: "onUpdate without params not supported."

`void onEnd()`
Throws logic_error: "onEnd without params not supported."

`CommandT* getCommand() const`
Returns the stored command pointer.

	