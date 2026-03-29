#include "DeselectMeshCallBack.h"
#include "CallbackRegister.h"
#include "../ViewPortsController.h"

static AutoRegisterCallback<DeselectMeshCallBack> autoRegisterDeselectMeshCallBack;

void DeselectMeshCallBack::execute()
{
    ViewPortsHolderContext::s_selectionController->clear();
    ViewPortsHolderContext::s_selectionController->setNewSelectionEvent(true);
}