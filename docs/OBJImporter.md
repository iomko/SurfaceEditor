# OBJImporter

The OBJImporter class is a concrete implementation of the BaseImporter interface. It is used to load geometry data from OBJ file formats.

### OBJImporter class

###### Methods
`void setFilePath(const std::string& filePath)` - Sets the path to the OBJ file that will be loaded.

`bool read()` - Reads and parses the contents of the OBJ file provided via setFilePath(). Returns true if the file was successfully parsed, otherwise returns false.

`std::vector<Mesh*> getMeshes()` - Returns a list of all constructed meshes parsed from the OBJ file.
