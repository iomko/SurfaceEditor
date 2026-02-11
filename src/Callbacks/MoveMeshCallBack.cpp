#include "MoveMeshCallBack.h"
#include "CallbackRegister.h"

#include "../ViewPortsController.h"
#include "../Utils/InterpolationUtils.h"
#include "../Utils/GeometryUtils.h"
#include "../Renderer/MaterialRegistry.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/CommandIDs.h"

static AutoRegisterCallback<MoveMeshCallBack> registerCallback;

void MoveMeshCallBack::execute(const MoveMeshParams &iParams)
{

    VertexParams vertexParams;
    vertexParams.mesh = iParams.mesh;
    vertexParams.transformMatrix = iParams.transformMatrix;
    vertexParams.moveBy = VertexParams::MoveBy::TRANSFORM_MATRIX;

    auto *moveVertexCommand = CommandRegistry::getCommand(MOVE_VERTEX_COMMAND);
    // chceme ist cez vsetky vertices a posunut ich o moveByVector

    if(moveVertexCommand)
    {
        const std::vector<ExtendedVertex *> &vertices = iParams.mesh->getHalfEdgeStructure()->m_vertices;

        for (ExtendedVertex *vertex : vertices)
        {
            vertexParams.vertex = vertex;
            moveVertexCommand->execute(vertexParams);
        }

    }
    iParams.mesh->calculateMeshBounds();
}