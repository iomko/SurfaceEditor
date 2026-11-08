#include "DeleteMeshCallBack.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/Structures/ExtendedHalfEdge.h"

#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/ViewPortsController.h"
#include "../../src/Callables/CallableRegistry.h"

static AutoRegisterCallback<DeleteMeshCallBack> registerDeleteMeshCallBack("DELETE_MESH_CALLBACK");
DeleteMeshCallBack::DeleteMeshCallBack() {}
void DeleteMeshCallBack::execute(const MeshParams &iParams)
{
    Mesh *selectedMesh = iParams.m_mesh;
    if (selectedMesh == nullptr)
    {
        return;
    }

    ViewPortsController* controller = ViewPortsHolderContext::s_viewPortsController.get();
    
    Scene *scene = controller->m_scene;
    Object* object = selectedMesh->m_parentObject;
    const bool removesObject = object != nullptr && object->getMeshes().size() == 1;
    scene->removeMesh(selectedMesh);
    ViewPortsHolderContext::s_selectionController->unregisterMesh(selectedMesh);

    auto removeOutlinerNode = [](void* data)
    {
        auto* callback = CallbackRegistry::instance().getCallback("REMOVE_OUTLINER_NODE_CALLBACK");
        auto* observable = dynamic_cast<Observable*>(callback);
        if (observable != nullptr)
        {
            RemoveOutlinerNodeParams params;
            params.data = data;
            observable->notifyObservers(params);
        }
    };
    removeOutlinerNode(selectedMesh);
    if (object != nullptr)
    {
        object->tryRemoveMesh(selectedMesh);
    }

    delete selectedMesh;

    if (removesObject)
    {
        removeOutlinerNode(object);
        controller->removeObject(object);
    }
}