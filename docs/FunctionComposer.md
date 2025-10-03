# FunctionComposer
FunctionComposer is a class to which we can add instances of the Callable class, and it then executes all of them sequentially.

### FunctionComposer class

###### Methods:

`template<typename CallableType> FunctionNode* initRoot()` – Adds a Callable that will always be executed first.

`template<typename CallableType> FunctionNode* addFunc(FunctionNode* functionNode)` – Adds a new Callable to an existing node, which will be executed after the Callable held by its parent node has completed.

`void execute()` – Traverses the entire tree, executing all stored Callables sequentially.

`void execute(const OpParams& inputParams)` – Similar to the standard execute() method, but provides input parameters that are sent to the root node.

