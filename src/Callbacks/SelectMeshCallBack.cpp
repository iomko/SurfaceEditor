#include "SelectMeshCallBack.h"
#include "CallbackRegister.h"
#include "../ViewPortsController.h"

static AutoRegisterCallback<SelectMeshCallBack> reg;

void SelectMeshCallBack::execute(const SelectionToolParams& iParams)
{
    ViewPortsHolderContext::s_selectionController->clear();
    
    for (auto& [mesh, face] : iParams.m_selectedData)
    {
        ViewPortsHolderContext::s_selectionController->registerMesh(mesh);
        ViewPortsHolderContext::s_selectionController->setNewSelectionEvent(true);
    }
}