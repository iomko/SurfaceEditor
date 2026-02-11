#include "MoveSelectedMeshesCallBack.h"
#include "CallbackRegister.h"

#include "../ViewPortsController.h"
#include "../Utils/InterpolationUtils.h"
#include "../Utils/GeometryUtils.h"
#include "../Renderer/MaterialRegistry.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"

static AutoRegisterCallback<MoveSelectedMeshesCallBack> registerMoveSelectedMeshesCallback;

void MoveSelectedMeshesCallBack::execute(const MoveSelectedMeshesParams &iParams)
{
    MoveMeshParams moveMeshParams;
    moveMeshParams.transformMatrix = iParams.transformMatrix;

    SelectionController *selectionController = ViewPortsHolderContext::s_selectionController;
    const SelectionHolder &selectionHolder = selectionController->getHolder();
    const std::vector<Mesh *> &selectedMeshes = selectionHolder.meshes;
    auto *moveMeshCommand = CommandRegistry::getCommand(MOVE_MESH_COMMAND);

    if(moveMeshCommand)
    {
        for (Mesh *selectedMesh : selectedMeshes)
        {
            // selectedMesh->m_transform[3] += glm::vec4(glm::vec3(moveMeshParams.moveByVector), 0.0f);
            selectedMesh->m_transform += moveMeshParams.transformMatrix;

            moveMeshParams.mesh = selectedMesh;
            moveMeshCommand->execute(moveMeshParams);
        }
    }
}