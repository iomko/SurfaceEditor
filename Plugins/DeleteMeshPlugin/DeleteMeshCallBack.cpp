#include "DeleteMeshCallBack.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/Structures/ExtendedHalfEdge.h"

#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/ViewPortsController.h"

static AutoRegisterCallback<DeleteMeshCallBack> registerDeleteMeshCallBack("DELETE_MESH_CALLBACK");
DeleteMeshCallBack::DeleteMeshCallBack() {}
void DeleteMeshCallBack::execute(const MeshParams &iParams)
{
    Mesh *selectedMesh = iParams.m_mesh;
    std::vector<ExtendedFace *> facesToDelete = selectedMesh->getHalfEdgeStructure()->m_faces;

    Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
    scene->deleteMeshFromOctrees(selectedMesh);
    delete selectedMesh;
    /*for (ExtendedFace *face : facesToDelete)
    {

        SingleFaceParams singleFaceParams;
        singleFaceParams.mesh = selectedMesh;
        singleFaceParams.face = face;

        auto *deleteFaceCommand = CommandRegistry::instance().getCommand("DELETE_FACE_COMMAND");
        if (deleteFaceCommand)
            deleteFaceCommand->execute(singleFaceParams);
    }*/
}