# AdditionLayer

A GUI layer class integrating ImGui interface for adding objects and fetching surface data.

Inherits from Layer, Observable, Observer, and `ui::components::Window` (ImGuiWrapper). Uses CommandRegistry to execute commands triggered by UI actions. The addition type to show (plane, cube or surface) arrives through `WindowLayerBus` as an `AdditionLayerState`.

###### Public Methods:
`AdditionLayer(const std::string& name)`
Constructor initializing the layer with a name and subscribing to `AdditionLayerState` events.

`void onImGuiRender()`
Renders the ImGui window for this layer, including UI for adding mesh objects and fetching surface data from OpenTopography via API key and coordinate inputs.

###### Protected Methods:
`void initWindowConfig()`
Builds the window configuration (size, title bar, flags).

`void initComponents()`
Creates the mesh, surface and dialog button components.

`std::unique_ptr<ui::placements::Placement> createPlacement()`
Returns a `CenteredPlacement`, so the window is always centered in the viewport.

###### Input values:
The input boxes and sliders do not own their values, they write into bindings owned by the layer:
- Subdivision and size use `ClampedBinding`, shared by the input box and the slider, limited to 1–100 and 0–100.
- Position uses `ReferenceBinding` into `m_position`.
- Surface bounds and the API key use `ReferenceBinding` into `m_surfaceParams` (`OpenTopoParams`), which is passed to `ADD_SURFACE_COMMAND` as is.
