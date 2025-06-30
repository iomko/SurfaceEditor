# OBJImporter

The OBJImporter class is a concrete implementation of the BaseImporter interface, specialized in loading geometry data from .obj files. It supports parsing multiple meshes (objects) within a single OBJ file, extracting vertex positions, normals, and polygon data to construct Mesh instances.

###### Public Methods
`void setFilePath(const std::string& filePath)`
Sets the path to the .obj file that will be loaded. This path is used during the read() operation.

`bool read()`
Reads and parses the contents of the .obj file provided via setFilePath().
It supports the following OBJ keywords:

v - for vertex positions

vn - for vertex normals

f - for polygon faces (with optional normal indices)

o - for separating multiple meshes within the same file

For each distinct object found, it creates a corresponding Mesh instance and stores it in the m_meshes vector if the build process was successful.

Returns true if the file was successfully parsed and at least one mesh was created; otherwise, returns false.

`std::vector<Mesh*> getMeshes()`
Returns a list of all successfully constructed meshes parsed from the .obj file.