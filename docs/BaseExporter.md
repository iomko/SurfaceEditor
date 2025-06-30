# BaseExporter
The BaseExporter class is an abstract interface for exporting mesh data.
It defines the essential methods that any concrete exporter must implement to handle mesh parsing and writing to an output format.

###### Public Methods
`virtual ~BaseExporter()`
Virtual destructor to ensure proper cleanup of derived classes.

`virtual bool write() = 0`
Pure virtual method to perform the export operation.
Returns true if the export was successful, otherwise false.

`virtual void parseMesh(Mesh* mesh) = 0`
Pure virtual method to parse and process the mesh data before export.
Takes a pointer to a Mesh object as input.
