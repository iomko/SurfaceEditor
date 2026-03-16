#include "DeselectMeshCallBack.h"
#include "CallbackRegister.h"
#include "../ViewPortsController.h"

static AutoRegisterCallback<DeselectMeshCallBack> autoRegisterDeselectMeshCallBack;
void DeselectMeshCallBack::execute(const SelectionToolParams& iParams)
{
    if (iParams.m_isClick)
    {
        ViewPortsHolderContext::s_selectionController->clear();
    }
    else
    {
        //TODO
    }

    // const auto& meshFaceHitPair = iParams.m_meshFaceHitPair;

    // SceneResources::MeshFacePair meshFacePair = meshFaceHitPair.first;

    // Mesh* mesh = meshFacePair.first;

    // ViewPortsHolderContext::s_selectionController->unregisterMesh(mesh);
}