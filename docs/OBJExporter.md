# OBJExporter
OBJExporter is a concrete class implementing the interface BaseExporter. It is designed to export one or more Mesh objects to the OBJ file format.  

### OBJExporter class

###### Methods
`bool write()` - Writes the mesh data to the specified OBJ file path.    
`void parseMesh(Mesh* mesh)` - Registers a Mesh pointer to be exported.  
`void setFilePath(const std::string& filePath)` - Sets the output file path where the OBJ data will be written.  
