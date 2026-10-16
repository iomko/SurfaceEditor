#include "MeshOutlinerAdderCallable.h"
#include "../../src/Callables/CallableRegistry.h"

#include "../../src/ViewPortsController.h"

#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Callbacks/CallbackRegister.h"

#include "../../src/Core/Profiler.h"

static AutoRegisterCallable<MeshOutlinerAdderCallable> reg("MESH_OUTLINER_ADDER_CALLABLE");

void MeshOutlinerAdderCallable::invoke(const MeshParams &input)
{
    SE_PROFILE_SCOPE_N("MeshOutlinerAdderCallable:invoke");
    Mesh *mesh = input.m_mesh;
    auto *addNodeCallback = CallbackRegistry::instance().getCallback(TemplateOutlinerNodeAdderCallbackIDManger::instance().GetIndex<Mesh>(false));
    Observable *observable = dynamic_cast<Observable *>(addNodeCallback);
    if (observable)
    {
        AddNewOutlinerNodeCallBackParams<Mesh> params;
        params.id = 0;
        params.name = input.m_name;
        params.data = mesh;

        observable->notifyObservers(params);
    }
}