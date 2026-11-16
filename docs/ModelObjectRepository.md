# ModelObjectRepository

`ModelObjectRepository` owns and manages `ModelObject` instances. The
`Application` owns the repository, so model-object lifetime is independent of
the scene and viewport controller.

The repository is for model ownership and lookup/lifecycle operations. The
`Scene` remains responsible for its mesh-based spatial indices and does not
own model objects.

## Methods

`ModelObject* addObject(std::unique_ptr<ModelObject> object)` – Transfers
ownership of a non-null model object to the repository and returns a
non-owning pointer to it. Passing `nullptr` throws `std::invalid_argument`.

`ModelObject* createObject(const std::string& name)` – Creates a model object
with the given name, stores it in the repository, and returns a non-owning
pointer to it.

`bool removeObject(ModelObject* object)` – Removes and destroys the specified
model only if it belongs to this repository and has no attached meshes.
Returns `false` if the pointer is not present or the model still contains
meshes.

`void clear()` – Removes and destroys all models currently owned by the
repository. Since each model destroys its attached meshes, ensure that scenes
and other systems no longer use those meshes before clearing the repository.

## Application access

Use `Application::getInstance().getModelObjectRepository()` to access the
application-owned repository. Do not store model ownership in
`ViewPortsController` or `Scene`.

See [ModelObject](ModelObject.md) for model and mesh ownership behavior.
