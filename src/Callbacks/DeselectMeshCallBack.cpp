#include "DeselectMeshCallBack.h"
#include "CallbackRegister.h"
#include "../ViewPortsController.h"

static AutoRegisterCallback<DeselectMeshCallBack> autoRegisterDeselectMeshCallBack;

void DeselectMeshCallBack::execute(const SelectionToolParams& iParams)
{
    ViewPortsHolderContext::s_selectionController->clear();
}