#include "DeleteMeshCallBack.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"
#include "../Structures/ExtendedHalfEdge.h"

#include "CallbackRegister.h"

static AutoRegisterCallback<DeleteMeshCallBack> registerDeleteMeshCallBack;
DeleteMeshCallBack::DeleteMeshCallBack() {}
void DeleteMeshCallBack::execute(const MeshParams &iParams)
{
    Mesh *selectedMesh = iParams.m_mesh;
    std::vector<ExtendedFace *> facesToDelete = selectedMesh->getHalfEdgeStructure()->m_faces;

    for (ExtendedFace *face : facesToDelete)
    {

        SingleFaceParams singleFaceParams;
        singleFaceParams.mesh = selectedMesh;
        singleFaceParams.face = face;

        auto *deleteFaceCommand = CommandRegistry::instance().getCommand(DELETE_FACE_COMMAND);
        if (deleteFaceCommand)
            deleteFaceCommand->execute(singleFaceParams);
    }
}