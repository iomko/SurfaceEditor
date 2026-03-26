#include "MoveMeshCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"

#include "../../src/ViewPortsController.h"
#include "../../src/Utils/InterpolationUtils.h"
#include "../../src/Utils/GeometryUtils.h"
#include "../../src/Renderer/MaterialRegistry.h"
#include "../../src/Commands/CommandRegistry.h"

static AutoRegisterCallback<MoveMeshCallBack> registerCallback("MOVE_MESH_CALLBACK");

void MoveMeshCallBack::execute(const MoveMeshParams &iParams)
{

    VertexParams vertexParams;
    vertexParams.mesh = iParams.mesh;
    vertexParams.transformMatrix = iParams.transformMatrix;
    vertexParams.moveBy = VertexParams::MoveBy::TRANSFORM_MATRIX;

    auto *moveVertexCommand = CommandRegistry::instance().getCommand("MOVE_VERTEX_COMMAND");
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