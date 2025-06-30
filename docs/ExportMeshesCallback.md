# ExportMeshesCallback

This class implements a callback for exporting selected meshes to an OBJ file.
It inherits from Callback<ImportExportMeshesParams> and Observer.
The callback uses an OBJExporter instance to handle the export process based on provided parameters.

###### Public Methods
`void execute(const ImportExportMeshesParams& params)`
Creates an OBJExporter instance,

Sets the export file path from the input parameters,

(Currently commented out) iterates over the selected meshes in the viewport context, parses each mesh for export,

Checks if there are no selected meshes and deletes the existing file if present,

Otherwise, writes the parsed meshes to the file.