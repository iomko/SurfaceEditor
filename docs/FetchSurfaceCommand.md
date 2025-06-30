# FetchSurfaceCommand

The FetchSurfaceCommand initiates the fetching of surface data based on parameters defined in OpenTopoParams.
It inherits from Command with OpenTopoParams as input and from Observable to notify observers about the fetch request.

This command serves as a trigger for observers to perform the surface data retrieval.

###### Public Methods
`void execute(const OpenTopoParams& params)`
Notifies observers with the provided parameters to start the surface fetching process.

`void undo()`
Currently not implemented; intended for future support of undo operations.

`static constexpr std::string_view getCommandName()`
Returns the command's unique name: "FetchSurfaceCommand".