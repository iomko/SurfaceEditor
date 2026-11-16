# ModelObject

`ModelObject` represents a named model made up of meshes. It groups meshes for
model-level operations and outliner presentation; it is not the scene's
spatial-index container.

## Ownership and lifetime

`Application` owns a `ModelObjectRepository`. The repository owns the
`ModelObject` instances, and each `ModelObject` owns the meshes currently
attached to it:

```text
Application
└── ModelObjectRepository
    └── ModelObject
        └── Mesh
```

Destroying a `ModelObject` deletes its attached meshes. `tryRemoveMesh`
detaches a mesh without deleting it, so callers can safely delete a detached
mesh themselves. Before removing a mesh that is referenced by the scene or
other systems, those references must first be removed or updated.

`Mesh::m_parentObject` points to the model that contains the mesh. Keep this
pointer consistent when attaching or detaching meshes.

## Constructors

`ModelObject()` creates a model named `DefaultObject`.

`ModelObject(std::string objectName)` creates a model with the supplied name.

## Methods

`void addMesh(Mesh* mesh)` – Attaches a non-null mesh if it is not already in
the model. Adding `nullptr` or an already attached mesh has no effect. This
method does not set `Mesh::m_parentObject`; callers that use this relationship
must set it themselves.

`bool containsMesh(Mesh* mesh) const` – Returns whether the given mesh pointer
is attached to this model.

`bool tryRemoveMesh(Mesh* mesh)` – Detaches the mesh and returns `true` if it
was found; otherwise leaves the model unchanged and returns `false`. It does
not delete the mesh.

`const std::vector<Mesh*>& getMeshes() const` – Returns a const reference to
the attached mesh pointers.

`std::string getObjectName() const` – Returns the model's name.

## Related documentation

- [ModelObjectRepository](ModelObjectRepository.md) owns and manages model
  objects for the lifetime of the application.
- [Scene](Scene.md) organizes meshes for scene queries and spatial indexing;
  it does not own model objects.
- [Mesh](Mesh.md) describes the mesh data grouped by a model.
