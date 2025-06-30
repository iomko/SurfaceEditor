# OBJExporter
OBJExporter is a concrete class implementing the abstract interface BaseExporter. It is designed to export one or more Mesh objects to the Wavefront OBJ file format.

###### Public Methods
`bool write()`
Writes the mesh data to the specified OBJ file path by invoking the private writeMeshData() method. Returns true on success.

`void parseMesh(Mesh* mesh)`
Registers a Mesh pointer to be exported. The mesh is appended to an internal list.

`void setFilePath(const std::string& filePath)`
Sets the output file path where the OBJ data will be written.