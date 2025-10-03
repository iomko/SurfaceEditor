# Layer

### Layer class
Layer serves as a base interface for modular application components that need to handle events, perform per-frame updates, and optionally render UI elements.  
###### Methods

`Layer(std::string name)` - Initializes the Layer with the specified name.  
  
`std::string getName()` - Returns the name of the layer.  
  
`virtual void onEvent(Event& event)` - Handles incoming events. Events can be marked as handled to prevent propagation.  

`virtual void onImGuiRender()` - Called once per frame for rendering `ImGui` components.  

`virtual void onUpdate()` - Called once per frame to update layer logic.  

`virtual void onDetach()` - Called once when the layer is removed from the LayerStack.  

`virtual void onAttach()` - Called once when the layer is added to the `LayerStack`.  

#### All the Layer class instances used in the project:

`AdditionLayer` - UI Window through which the user can execute the AddPlaneCommand or FetchSurfaceCommand.  
`ImGuiLayer` - Layer that is responsible for setting all the necessary imgui data before any layers are drresponsible for setting all the necessary imgui data before any layers are drawn.  
`ImportExportLayer` - UI Window through which the user can either export or import models into the application.  
`Modifierslayer` - UI Window through which the user can either execute the SolidifyMeshesCommand or CreatePrintCommand.  
`OutlinerLayer` - UI Window where user can see all the objects being added to the scene.  
`PrintableMeshSettingsPopUpLayer` - UI Pop-Up Window through which the user can modify the settings of selected PrintableMesh.  
`RemovalLayer` - UI Window through which the user can execute the DeleteSelectedFacesCommand or DeleteSelectedMeshesCommand.  
`SculptToolsLayer` - UI Window through which the user can execute the BrushToolCommand.  
`SelectionLayer` - UI Window through which the user can choose between the two different types of selections (MeshSelection or FaceSelection).  

### LayerStack class
The LayerStack manages the collection of layers and overlays in the application. It provides an interface for adding, removing, and iterating through layers.
###### Methods

`void addLayer(Layer* layer)` – Adds a Layer to the stack. Layers are inserted below overlays.  

`void removeLayer(Layer* layer)` – Removes the specified Layer from the stack.  

`void addOverlay(Layer* overlay)` – Adds an overlay on top of all layers. Overlays are always rendered last.  

`void removeOverlay(Layer* overlay)` – Removes the specified overlay from the stack.  

`std::vector<Layer*>::iterator begin()` – Returns an iterator to the beginning of the layer stack (for iteration).  

`std::vector<Layer*>::iterator end()` – Returns an iterator to the end of the layer stack.  
