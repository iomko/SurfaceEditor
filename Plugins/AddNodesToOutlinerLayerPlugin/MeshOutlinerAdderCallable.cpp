#include "MeshOutlinerAdderCallable.h"
#include "../../src/Callables/CallableRegistry.h"

#include "../../src/ViewPortsController.h"

#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Callbacks/CallbackRegister.h"

static AutoRegisterCallable<MeshOutlinerAdderCallable> reg;

void MeshOutlinerAdderCallable::invoke(const MeshParams &input)
{
    Mesh *mesh = input.m_mesh;
    auto *addNodeCallback = CallbackRegistry::instance().getCallback(TemplateOutlinerNodeAdderCallbackIDManger::instance().GetIndex<Mesh>(false));
    Observable *observable = dynamic_cast<Observable *>(addNodeCallback);
    if (observable)
    {
        AddNewOutlinerNodeCallBackParams<Mesh> params;
        params.id = 0;
        params.name = "Mesh";
        params.data = mesh;

        observable->notifyObservers(params);
    }
}