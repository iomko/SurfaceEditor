#pragma once
#include "../Params/OperationParams.h"
#include "../ViewPortsController.h"
#include "../Utils/InterpolationUtils.h"
#include "../Utils/GeometryUtils.h"
#include "../Renderer/MaterialRegistry.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/MoveVertexCommand.h"

class MoveMeshCallBack : public Callback<MoveMeshParams>, public Observer
{
public:
    MoveMeshCallBack(CommandRegistry* commandRegistry) : m_commandRegistry(commandRegistry) {}

    void execute(const MoveMeshParams& iParams) override {

        VertexParams vertexParams;
        vertexParams.mesh = iParams.mesh;
        MoveVertexCommand* moveVertexCommand = m_commandRegistry->getCommand<MoveVertexCommand>();
        //chceme ist cez vsetky vertices a posunut ich o moveByVector

        const std::vector<ExtendedVertex*>& vertices = iParams.mesh->getHalfEdgeStructure()->m_vertices;

        for(ExtendedVertex* vertex : vertices) {
            vertexParams.newPosition = vertex->m_position + iParams.moveByVector;
            vertexParams.vertex = vertex;
            moveVertexCommand->execute(vertexParams); 
        }

        iParams.mesh->calculateMeshBounds();

    }

private:
    CommandRegistry* m_commandRegistry = nullptr;
};
