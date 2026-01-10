#include "MeshOutlinerAdderCallable.h"
#include "CallableRegistry.h"

#include "../ViewPortsController.h"

#include "../Callbacks/CallbackIDs.h"
#include "../Callbacks/CallbackRegister.h"

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