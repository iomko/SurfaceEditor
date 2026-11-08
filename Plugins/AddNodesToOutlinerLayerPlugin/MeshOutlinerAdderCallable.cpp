#include "MeshOutlinerAdderCallable.h"
#include "../../src/Callables/CallableRegistry.h"

#include "../../src/ViewPortsController.h"

#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Callbacks/CallbackRegister.h"

#include "../../src/Core/Profiler.h"
#include "../../src/UI/WindowLayerBus.h"

static AutoRegisterCallable<MeshOutlinerAdderCallable> reg("MESH_OUTLINER_ADDER_CALLABLE");

void MeshOutlinerAdderCallable::invoke(const MeshParams &input)
{
    SE_PROFILE_SCOPE_N("MeshOutlinerAdderCallable:invoke");
    Mesh *mesh = input.m_mesh;
    ViewPortsController* controller = ViewPortsHolderContext::s_viewPortsController.get();
    Object* object = input.m_object;
    if (object == nullptr && controller != nullptr)
    {
        object = mesh->m_parentObject;
    }
    if (mesh == nullptr || object == nullptr)
    {
        throw std::logic_error("A mesh must belong to an object before adding it to the outliner.");
    }

    auto* addObjectCallback = CallbackRegistry::instance().getCallback(
        TemplateOutlinerNodeAdderCallbackIDManger::instance().GetIndex<Object>(false));
    auto* addObjectObservable = dynamic_cast<Observable*>(addObjectCallback);
    if (addObjectObservable == nullptr)
    {
        throw std::logic_error("Object outliner callback is not registered.");
    }
    AddNewOutlinerNodeCallBackParams<Object> objectParams;
    objectParams.name = object->getObjectName();
    objectParams.data = object;
    addObjectObservable->notifyObservers(objectParams);

    auto* layerState = dynamic_cast<OutlinerLayerState*>(objectParams.state);
    if (layerState == nullptr)
    {
        return;
    }

    OutlinerNodeConcept* objectNode = nullptr;
    for (OutlinerNodeConcept* node : layerState->m_nodes)
    {
        auto* typedNode = dynamic_cast<OutlinerNode<Object>*>(node);
        if (typedNode != nullptr && typedNode->m_data == object)
        {
            objectNode = node;
            break;
        }
    }
    if (objectNode == nullptr)
    {
        throw std::logic_error("Object was not added to the outliner.");
    }

    auto* addMeshCallback = CallbackRegistry::instance().getCallback(
        TemplateOutlinerNodeAdderCallbackIDManger::instance().GetIndex<Mesh>(true));
    auto* addMeshObservable = dynamic_cast<Observable*>(addMeshCallback);
    if (addMeshObservable == nullptr)
    {
        throw std::logic_error("Mesh outliner callback is not registered.");
    }
    AddChildOutlinerNodeCallBackParams<Mesh> meshParams;
    meshParams.id = 0;
    meshParams.name = input.m_name;
    meshParams.data = mesh;
    meshParams.parent = objectNode;
    addMeshObservable->notifyObservers(meshParams);
}