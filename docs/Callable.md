	
# Callable

Callable is a class that serves as a lambda-like function. 
To execute this function, the invoke method is used, which can take input and output as parameters. 
These callable classes can be executed sequentially one after another using the FunctionComposer class. 
The Callable class is a base class, from which you need to inherit in order to create an instance. 
It is also necessary to override the invoke method.


### Callable class

###### Possible methods:
`virtual void invoke(const IParams& input, OParams& output) = 0`  
`virtual void invoke(OParams& output) = 0`  
`virtual void invoke(const IParams& input) = 0`  
`virtual void invoke() = 0`  


All the Callable class instances used in the project:  

`EdgesVaoInitCallable` – Adds all necessary data to the line buffer. This buffer is then sent to the GPU for rendering.  

`FaceVaoInitCallable` – Adds all necessary data to the mesh buffer. This buffer is then sent to the GPU for rendering.  

`FacesVaoInitCallable` – Adds all necessary data to the mesh buffer. This buffer is then sent to the GPU for rendering.  

`MeshVaoInitCallable` – Adds all necessary data to both the mesh and line buffers. These buffers are then sent to the GPU for rendering.  

`FetchedSurfaceVertexGenCallable` – Retrieves data from OpenTopography in the form of a heightmap, which is then used to generate a new surface.  

`MeshOutlinerAdderCallable` – Adds a mesh to the Outliner layer.  

`PlaneVertexGenCallable` – Creates a new plane-type mesh based on the input parameters.  

`SceneFacesAdderCallable` – Adds the faces of a given mesh to the scene.  

`SceneMeshAdderCallable` – Adds all faces of a given mesh to the scene.  
