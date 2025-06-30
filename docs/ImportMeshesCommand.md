# ImportMeshesCommand

The ImportMeshesCommand triggers the import of mesh data using parameters specified in ImportExportMeshesParams.
It inherits from Command templated with ImportExportMeshesParams and from Observable to notify observers when the command is executed.

This command is designed to initiate the import process, delegating the actual import work to observers.

###### Public Methods
`void execute(const ImportExportMeshesParams& params)`
Notifies observers with the given parameters to start importing meshes.

`static constexpr std::string_view getCommandName()`
Returns the unique name of the command: "ImportMeshesCommand".