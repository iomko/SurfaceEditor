# ExportMeshesCommand

The ExportMeshesCommand handles the export operation of meshes based on the given parameters.
It inherits from Command with ImportExportMeshesParams as input and from Observable to notify observers about the export event.

This command triggers mesh export logic by passing the export parameters to observers.

###### Public Methods
`void execute(const ImportExportMeshesParams& params)`
Notifies observers with the export parameters, initiating the export process.

`static constexpr std::string_view getCommandName()`
Returns the unique command name: "ExportMeshesCommand".