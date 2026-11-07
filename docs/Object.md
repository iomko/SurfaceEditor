# Object

`Object` groups one or more meshes for ownership and outliner presentation. It does not replace the scene's mesh-based spatial indices.

The viewport controller owns the created objects. Destroying an object deletes any meshes still attached to it. Removing a mesh from an object detaches it, so callers that explicitly delete a mesh can do so without the object deleting it again.

Meshes created individually are placed in their own objects. Meshes imported from one file share a single object and appear as child entries in the outliner.
