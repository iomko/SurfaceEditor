# Callback

### CallbackConcept
An abstract base interface defining the callback concept with three execute method variants:

- without parameters

- with input parameters (OpParams)

- with input and output parameters (OpParams).

### Callback&lt;IParams, OParams&gt;
A templated abstract class extending CallbackConcept for callbacks with input and output parameters of types IParams and OParams.
Supports execution with these parameters, while other execute variants throw exceptions if unsupported.

###### Public Methods
`virtual void execute(const IParams& iParams, OParams& oParams)`
Pure virtual method to execute the callback with input and output parameters.

`void execute(const OpParams& iParams, OpParams& oParams)`
Overrides base method by casting parameters to proper types and calling the implemented version.

`void execute()`
Throws a logic error if called without any parameters.

`void execute(const OpParams&)`
Throws a logic error if called with only input parameters (no output).

### Callback&lt;IParams, OpParams&gt;
A templated abstract class specialization for callbacks with input parameters IParams but no output parameters.

###### Public Methods
`virtual void execute(const IParams& iParams)`
Pure virtual method to execute the callback with input parameters.

`void execute(const OpParams& iParams)`
Overrides base method by casting input parameters and calling the implemented version.

`void execute()`
Throws a logic error if called without parameters.

`void execute(const OpParams&, OpParams&)`
Throws a logic error if called with both input and output parameters.

### Callback&lt;OpParams, OpParams&gt;
Full specialization for callbacks without parameters.

###### Public Methods
`virtual void execute()`
Pure virtual method to execute the callback without parameters.

`void execute(const OpParams&)`
Throws a logic error if called with input parameters.

`void execute(const OpParams&, OpParams&)`
Throws a logic error if called with input and output parameters.

# ComposedCallback

### ComposedCallback&lt;IParams&gt;
Templated class implementing CallbackConcept that composes multiple functions via a FunctionComposer object and supports execution with input parameters IParams.

###### Public Methods
`ComposedCallback(const FunctionComposer& functionComposer)`
Constructor that initializes the composed callback with a given FunctionComposer.

`void execute(const OpParams& iParams)`
Casts and forwards input parameters to the internal FunctionComposer for execution.

`void execute()`
Throws a logic error if called without parameters.

`void execute(const OpParams&, OpParams&)`
Throws a logic error if called with input and output parameters.

### ComposedCallback&lt;OpParams&gt;
Full specialization for parameterless composed callbacks.

###### Public Methods
`ComposedCallback(const FunctionComposer& functionComposer)`
Constructor that initializes with a FunctionComposer.

`void execute()`
Executes the composed function sequence without parameters.

`void execute(const OpParams&)`
Throws a logic error if called with input parameters.

`void execute(const OpParams&, OpParams&)`
Throws a logic error if called with input and output parameters.