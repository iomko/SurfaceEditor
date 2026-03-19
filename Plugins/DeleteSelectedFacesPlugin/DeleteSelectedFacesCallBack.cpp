#include "DeleteSelectedFacesCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/Utils/ContainerUtils.h"
#include "../../src/Structures/ExtendedHalfEdge.h"
#include "../../src/ViewPortsController.h"

static AutoRegisterCallback<DeleteSelectedFacesCallBack> registerDeleteSelectedFacesCallBack("DELETE_SELECTED_FACES_CALLBACK");
DeleteSelectedFacesCallBack::DeleteSelectedFacesCallBack() {} 
void DeleteSelectedFacesCallBack::execute()
{
    // Get selected meshes
    SelectionController *selectionController = ViewPortsHolderContext::s_selectionController;
    const SelectionHolder &selectionHolder = selectionController->getHolder();
    Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
    const std::vector<Mesh *> &selectedMeshes = selectionHolder.meshes;

    for (Mesh *selectedMesh : selectedMeshes)
    {
        const std::vector<ExtendedFace *> &selectedFaces = selectionHolder.faces.find(selectedMesh)->second;

        // Go through each selected Face
        while (!selectedFaces.empty())
        {
            ExtendedFace *selectedFace = selectedFaces.back();

            SingleFaceParams singleFaceParams;
            singleFaceParams.mesh = selectedMesh;
            singleFaceParams.face = selectedFace;

            auto *deleteFaceCommand = CommandRegistry::instance().getCommand("DELETE_FACE_COMMAND");
            if (deleteFaceCommand)
                deleteFaceCommand->execute(singleFaceParams);

            // Delete face/mesh from selection
            if (selectedMesh->m_halfEdgeStructure->m_faces.empty())
            {
                selectionController->unregisterMesh(selectedMesh);
            }
            else
            {
                selectionController->unregisterFace(selectedMesh, selectedFace);
            }
        }

        selectedMesh->calculateMeshBounds();
    }
}