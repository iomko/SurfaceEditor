# Callback

Callback is a class that is executed automatically when something happens. 
For example, when a new plane-type mesh is added to the scene, an instance of AddPlaneCallback is automatically triggered. 
Currently, callbacks in the project are always used in connection with instances of the Command class.

Similar to the Callable class, the Callback class is a base class from which you need to inherit in order to create an instance.
It is also necessary to override the execute method.


### Callback class

###### Possible methods:  
`virtual void execute(const IParams& iParams, OParams& oParams) = 0`  
`virtual void execute(const IParams& iParams) = 0`  
`virtual void execute() = 0`   

Additionally, there is a ComposedCallback class, which takes a FunctionComposer as a constructor parameter. 
The FunctionComposer is pre-filled with the required callable instances, and this class then internally triggers sequential execution of the callables within its execute method.


### ComposedCallback class

`void execute()`  


All the Callback class instances used in the project:

`AddPlaneCallback` – Creates and adds a plane-type mesh to the scene.  

`BrushToolCallback` – Allows deformation of the surface.  

`ConnectEdgesCallback` – Connects two mesh edges together.  

`CreatePrintStructureCallback` – Creates and adds a PrintableMesh to the scene.  

`DeleteSelectedFacesCallback` – Deletes all selected faces from the scene.  

`DeleteSelectedMeshesCallback` – Deletes all selected meshes from the scene.  

`DeselectFaceCallback` – If it exists, unregisters a selected face from the selected faces list.  

`DeselectMeshCallback` – If it exists, unregisters a selected mesh from the selected meshes list.  

`ExportMeshesCallback` – Exports selected meshes to the OBJ format.  

`FetchSurfaceCallback` – Creates and adds to the scene a mesh whose data was retrieved from the OpenTopography source.  

`ImportMeshesCallback` – Imports meshes from the OBJ format into the scene.  

`SelectFaceCallback` – Selects a face chosen in the scene.  

`SelectMeshCallback` – Selects a mesh chosen in the scene.  

`SelectionLayerCallback` – Chooses the correct tool from the Selection Layer and applies it.  

`SolidifyMeshesCallback` – Adds thickness (depth) to a mesh.  

`MoveVertexCallback` – Updates the position, rotation and scale of vertex based on the gizmo transformation matrix.

`MoveMeshCallback` – Updates the position, rotation and scale of mesh based on the gizmo transformation matrix using `MoveVertexCallback`.

`MoveSelectedMeshesCallback` – Applies `MoveMeshCallback` to each selected mesh.

`MoveSelectedFacesCallback` – Applies `MoveVertexCallback` to each selected face and its vertices.

`HandleGizmoCallback` – Handles all the gizmo translation, rotation and scaling logic with use of transformation matrices.