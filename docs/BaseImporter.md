# BaseImporter

The BaseImporter class serves as an abstract interface for importing mesh data.
It declares the core methods required for reading data from a source and retrieving the imported meshes.

###### Public Methods
`virtual ~BaseImporter()`
Virtual destructor to ensure proper cleanup of derived importer classes.

`virtual bool read() = 0`
Pure virtual method to read and import mesh data from the source.
Returns true if the read operation was successful, otherwise false.

`virtual std::vector<Mesh*> getMeshes() = 0`
Pure virtual method that returns a vector containing pointers to the imported Mesh objects.