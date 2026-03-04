#include "DeleteSelectedMeshesCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/Commands/CommandIDs.h"
#include "../../src/ViewPortsController.h"

static AutoRegisterCallback<DeleteSelectedMeshesCallBack> registerDeleteSelectedMeshesCallBack;
DeleteSelectedMeshesCallBack::DeleteSelectedMeshesCallBack() {}
void DeleteSelectedMeshesCallBack::execute()
{
    const SelectionHolder& selectionsHolder = ViewPortsHolderContext::s_selectionController->getHolder();
    const std::vector<Mesh*>& selectedMeshes = selectionsHolder.meshes;

    while(!selectedMeshes.empty())
    {
        Mesh* mesh = selectedMeshes.back();

        MeshParams meshParams;
        meshParams.m_mesh = mesh;

        auto* deleteMeshCommand = CommandRegistry::instance().getCommand(DELETE_MESH_COMMAND);
        if(deleteMeshCommand) deleteMeshCommand->execute(meshParams); 

        ViewPortsHolderContext::s_selectionController->unregisterMesh(mesh);
    }
}