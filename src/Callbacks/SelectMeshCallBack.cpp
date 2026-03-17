#include "SelectMeshCallBack.h"
#include "CallbackRegister.h"
#include "../ViewPortsController.h"

static AutoRegisterCallback<SelectMeshCallBack> reg;

void SelectMeshCallBack::execute(const SelectionToolParams& iParams)
{
    for (auto mesh : iParams.m_selectedMeshes)
    {
        ViewPortsHolderContext::s_selectionController->registerMesh(mesh);
    }
}