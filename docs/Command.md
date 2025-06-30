# Command
Generic command interface for executing and undoing operations with optional strongly-typed input and output parameters.

### CommandConcept
Abstract base interface for all commands.

Defines standard polymorphic execution and undo functionality.

###### Public Methods:
`virtual ~CommandConcept()`
Virtual destructor.

`virtual void execute()`
Executes the command with no parameters.

`virtual void execute(const OpParams& iParams)`
Executes the command with input parameters.

`virtual void execute(const OpParams& iParams, OpParams& oParams)`
Executes the command with both input and output parameters.

`virtual void undo()`
Performs undo operation if implemented by derived class.

### Command<IParams, OParams>
Primary template class for commands that use both input and output parameter structures.

Derives from CommandConcept. You must override the typed execute(const IParams&, OParams&) in derived classes.

###### Public Methods:
`void undo()`
Default empty undo operation.

`void execute()`
Default no-op.

`void execute(const OpParams& iParams)`
Default no-op.

`virtual void execute(const IParams& iParams, OParams& oParams)`
Pure virtual method to be implemented for actual command logic.

`void execute(const OpParams& iParams, OpParams& oParams)`
Casts to specific types and forwards to execute(const IParams&, OParams&).

### Command<IParams, OpParams>
Specialization for commands that only need input parameters but no output.

You must override the typed execute(const IParams&).

###### Public Methods:
`void undo()`
Default empty undo operation.

`void execute()`
Default no-op.

`void execute(const OpParams& iParams, OpParams& oParams)`
Default no-op.

`virtual void execute(const IParams& iParams)`
Pure virtual method to be implemented by derived class.

`void execute(const OpParams& iParams)`
Casts input and forwards to execute(const IParams&).

### Command<OpParams>
Specialization for commands that take no input and produce no output.

You must override the parameterless execute().

###### Public Methods:
`void undo()`
Default empty undo operation.

`virtual void execute()`
Pure virtual method for executing the command.

`void execute(const OpParams&)`
Default no-op.

`void execute(const OpParams& iParams, OpParams& oParams)`
Default no-op.