#include "MoveSelectedMeshesCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"

#include "../../src/ViewPortsController.h"
#include "../../src/Utils/InterpolationUtils.h"
#include "../../src/Utils/GeometryUtils.h"
#include "../../src/Renderer/MaterialRegistry.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/Commands/CommandIDs.h"

static AutoRegisterCallback<MoveSelectedMeshesCallBack> registerMoveSelectedMeshesCallback;

void MoveSelectedMeshesCallBack::execute(const MoveSelectedMeshesParams &iParams)
{
    MoveMeshParams moveMeshParams;
    moveMeshParams.transformMatrix = iParams.transformMatrix;

    SelectionController *selectionController = ViewPortsHolderContext::s_selectionController;
    const SelectionHolder &selectionHolder = selectionController->getHolder();
    const std::vector<Mesh *> &selectedMeshes = selectionHolder.meshes;
    auto *moveMeshCommand = CommandRegistry::instance().getCommand(MOVE_MESH_COMMAND);

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