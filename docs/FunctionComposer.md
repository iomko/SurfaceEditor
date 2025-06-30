
# FunctionComposer
The FunctionComposer class enables hierarchical composition and execution of multiple callable objects. It constructs a tree of function nodes, each performing a transformation on input parameters and passing results to its children.

###### Public Methods
`template&lt;typename CallableType&gt; FunctionNode* initRoot()`
Initializes the root node of the function tree with a callable object of type CallableType. Returns a pointer to the created root node. Sets an internal flag to determine if input is required.

`template&lt;typename CallableType&gt; FunctionNode* addFunc(FunctionNode* functionNode)`
Adds a new child node with a callable object of type CallableType to the specified parent node. Returns a pointer to the newly created node.

`void execute()`
Executes the function composition starting from the root node. This overload should only be called if the root node does not require input parameters. Displays an error if input is required.

`void execute(const OpParams& inputParams)`
Executes the function composition starting from the root node using the provided input parameters. This overload should only be used if the root node does require input. Displays an error otherwise.

`~FunctionComposer()`
Destroys the FunctionComposer and deallocates the root node.

# FunctionNode
The FunctionNode class is a lightweight structure used to represent a node in the function composition tree. Each node holds a callable object and a list of its child nodes.
