#include "SelectMeshCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/ViewPortsController.h"

static AutoRegisterCallback<SelectMeshCallBack> reg("SELECT_MESH_CALLBACK");

void SelectMeshCallBack::execute(const SelectionToolParams& iParams)
{
    ViewPortsHolderContext::s_selectionController->clear();
 
    if (!iParams.m_select)
    {
        ViewPortsHolderContext::s_selectionController->setNewSelectionEvent(true);
        return;
    }

    for (auto& [mesh, face] : iParams.m_selectedData)
    {
        ViewPortsHolderContext::s_selectionController->registerMesh(mesh);
        ViewPortsHolderContext::s_selectionController->setNewSelectionEvent(true);
    }
}