# AddPlaneCallback
The AddPlaneCallback class represents a callback that invokes a composed sequence of operations using PlaneParams as input. It inherits from ComposedCallback<PlaneParams> and implements the Observer interface to support integration with event systems (e.g., UI events or state change triggers).

###### Public Methods
`AddPlaneCallback(const FunctionComposer& functionComposer)`
Constructs an AddPlaneCallback using the given FunctionComposer. The provided function composer defines the chain of operations to be executed when the callback is triggered.