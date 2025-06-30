# FetchSurfaceCallBack
The FetchSurfaceCallBack class is a specialized callback designed to handle operations involving OpenTopoParams.
It inherits from ComposedCallback<OpenTopoParams>, enabling execution of a composed set of functions via a FunctionComposer.
Additionally, it implements the Observable and Observer interfaces for event notification and observation.

###### Public Methods
`FetchSurfaceCallBack(const FunctionComposer& functionComposer)`
Constructs the callback with a given FunctionComposer that defines the sequence of functions to be executed when the callback is triggered.