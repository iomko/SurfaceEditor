# Command
Command is a class that represents the execution of specific functionality within the application. Currently, all instances of this class are also associated with their corresponding Callback instance.

### Command class

###### Possible methods:
`virtual void undo()`

`virtual void execute() = 0`

`virtual void execute(const IParams& iParams) = 0`

`virtual void execute(const IParams& iParams, OParams& oParams) = 0`


#### All the Command class instances used in the project:

`AddPlaneCommand` – Creates and adds a plane-type mesh to the scene.  

`BasicSculptToolCommand` – Allows deformation of the surface.  

`BrushToolCommand` – Currently replaces the BasicSculptToolCommand class.  

`CreatePrintCommand` – Creates and adds a PrintableMesh to the scene.  

`DeleteSelectedFacesCommand` – Deletes all selected faces from the scene.  

`DeleteSelectedMeshesCommand` – Deletes all selected meshes from the scene.  

`DeselectFaceCommand` – If it exists, unregisters a selected face from the selected faces list.  

`DeselectMeshCommand` – If it exists, unregisters a selected mesh from the selected meshes list.  

`ExportMeshesCommand` – Exports selected meshes to the OBJ format.  

`FetchSurfaceCommand` – Creates and adds to the scene a mesh whose data was retrieved from the OpenTopography source.  

`ImportMeshesCommand` – Imports selected meshes from the OBJ format.  

`SelectFaceCommand` – Selects a face chosen in the scene.  

`SelectMeshCommand` – Selects a mesh chosen in the scene.  

`SolidifyMeshesCommand` – Adds thickness (depth) to a mesh.  

# CommandRegistry

CommandRegistry is a class that stores all registered instances of the Command type.

### CommandRegistry class

###### Methods:
`template<typename CommandT, typename... Args> void registerCommand(Args&&... args)` – Registers a new command in the registry.

`template<typename CommandT> CommandT* getCommand()` – Retrieves a specific command.

`void deleteRegistry()` – Clears the entire registry, deleting all stored commands.
