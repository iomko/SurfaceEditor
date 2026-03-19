#include "MoveSelectedFacesCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/ViewPortsController.h"
#include "../../src/Utils/InterpolationUtils.h"
#include "../../src/Utils/GeometryUtils.h"
#include "../../src/Renderer/MaterialRegistry.h"
#include "../../src/Commands/CommandRegistry.h"

static AutoRegisterCallback<MoveSelectedFacesCallBack> moveSelectedFacesCallBackRegister("MOVE_SELECTED_FACES_CALLBACK");

MoveSelectedFacesCallBack::MoveSelectedFacesCallBack() {}

void MoveSelectedFacesCallBack::execute(const MoveSelectedFacesParams &iParams) 
{
    glm::vec3 moveByVector = iParams.moveByVector;

    // Get selected meshes
    SelectionController *selectionController = ViewPortsHolderContext::s_selectionController;
    const SelectionHolder &selectionHolder = selectionController->getHolder();
    Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
    const std::vector<Mesh *> &selectedMeshes = selectionHolder.meshes;

    for (Mesh *selectedMesh : selectedMeshes)
    {
        const std::vector<ExtendedFace *> &selectedFaces = selectionHolder.faces.find(selectedMesh)->second;

        // musime ziskat vsetky unique vertices
        std::unordered_set<ExtendedVertex *> verticesToMove;

        for (ExtendedFace *face : selectedFaces)
        {
            face->m_transform[3] += glm::vec4(glm::vec3(iParams.moveByVector), 0.0f);

            for (auto it = face->faceVertexBegin(); it != face->faceVertexEnd(); ++it)
            {

                ExtendedVertex *vertex = &(*it);
                verticesToMove.insert(vertex);
            }
        }

        VertexParams vertexParams;
        vertexParams.mesh = selectedMesh;
        vertexParams.newPosition = moveByVector;
        vertexParams.moveBy = VertexParams::MoveBy::VECTOR;
        for (ExtendedVertex *vertex : verticesToMove)
        {
            vertexParams.vertex = vertex;

            auto *moveVertexCommand = CommandRegistry::instance().getCommand("MOVE_VERTEX_COMMAND");
            if (moveVertexCommand)
            {
                moveVertexCommand->execute(vertexParams);
            }
        }
        selectedMesh->calculateMeshBounds();
    }
}
