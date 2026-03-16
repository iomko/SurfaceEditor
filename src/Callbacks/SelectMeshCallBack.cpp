#include "SelectMeshCallBack.h"
#include "CallbackRegister.h"
#include "../ViewPortsController.h"

static AutoRegisterCallback<SelectMeshCallBack> reg;

void SelectMeshCallBack::execute(const SelectionToolParams& iParams)
{
    if (iParams.m_isClick)
    {
        const auto& meshFaceHitPair = iParams.m_meshFaceHitPair;

        SceneResources::MeshFacePair meshFacePair = meshFaceHitPair.first;

        Mesh* mesh = meshFacePair.first;

        ViewPortsHolderContext::s_selectionController->registerMesh(mesh);
    }
    else
    {
        //TODO
    }
}