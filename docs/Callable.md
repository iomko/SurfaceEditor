	
# Callable
	
### CallableConcept
The CallableConcept class defines a polymorphic interface for command-like objects that can be executed with input parameters and optionally return output. This interface is used to decouple the execution logic from specific parameter types.

###### Public Methods
`virtual ~CallableConcept()`
Virtual destructor ensures proper cleanup in derived classes.

`virtual OpParams* execute(const OpParams& input)`
Executes the command using the provided input parameters and returns a pointer to the resulting output (if any). This method must be overridden by all derived classes.

### Callable&lt;IParams, OParams&gt;
The Callable template class represents a type-safe executable unit that takes input parameters of type IParams and produces output of type OParams. It inherits from CallableConcept and provides a type-safe casting and delegation mechanism.

###### Template Parameters
`IParams` - Input parameter type, derived from OpParams.

`OParams` - Output parameter type, derived from OpParams.

###### Public Methods
`OpParams* execute(const OpParams& input)`
Casts the input to IParams, allocates an OParams object, and calls invoke().

`virtual void invoke(const IParams& input, OParams& output)`
Must be implemented by subclasses to define the core logic of the command.

### Callable&lt;void, OParams&gt;
Specialization for commands that do not require input parameters but produce an output.

###### Public Methods
`OpParams* execute(const OpParams&)`
Ignores the input and only creates and fills the OParams output using invoke().

`virtual void invoke(OParams& output)`
Must be implemented to produce output without input.

### Callable&lt;IParams, void&gt;
Specialization for commands that require input parameters but produce no output.

###### Public Methods
`OpParams* execute(const OpParams& input)`
Casts the input to IParams and executes the logic via invoke(). Returns nullptr.

`virtual void invoke(const IParams& input)`
Must be implemented to define logic with input only.

### Callable&lt;void, void&gt;
Specialization for commands that require no input and produce no output.

###### Public Methods
`OpParams* execute(const OpParams&)`
Ignores input, calls invoke(), and returns nullptr.

`virtual void invoke()`
Must be implemented to define input/output-free behavior.