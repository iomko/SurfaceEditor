#pragma once
#include "Params/OperationParams.h"
#include "Callbacks/Callback.h"
#include "Patterns/Observer.h"
#include "Utils/ContainerUtils.h"
#include "../Renderer/BufferStorages.h"
#include "../Renderer/MaterialRegistry.h"
#include "Renderer/Renderer.h"

class ConnectEdgesCallBack : public Callback<EdgeConnectionsParams>, public Observer
{
public:
	void execute(const EdgeConnectionsParams& iParams) override
	{
        ExtendedEdge* firstEdge = iParams.firstEdge;
        ExtendedEdge* secondEdge = iParams.secondEdge;
        Mesh* mesh = iParams.mesh;

        deleteEdgeVaoData(mesh, firstEdge);
        mesh->m_halfEdgeStructure->connectEdges(iParams.firstEdge, iParams.secondEdge);
    }

private:

	void deleteEdgeVaoData(Mesh* mesh, ExtendedEdge* edge)
	{

        Material* defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");
        if(auto opt = mesh->bufferLayout.getLineBufferStorage(defaultLineMaterial)) {

            LineBufferStorage& lineBufferStorage = opt->get();
            std::vector<BufferStorageDataType::LineVertex>& lineBufferVertices = lineBufferStorage.data.vertices;

            if (edge->m_EdgeLineIndex != (lineBufferVertices.size() - 2))
            {
                utils::containers::reverseSubrange(lineBufferVertices, lineBufferVertices.size() - 2, lineBufferVertices.size() - 1);
                for (int i = edge->m_EdgeLineIndex; i <= edge->m_EdgeLineIndex + 1; ++i)
                {
                    utils::containers::swapLastAndPop(lineBufferVertices, i);
                }

                mesh->m_halfEdgeStructure->m_edges.back()->m_EdgeLineIndex = edge->m_EdgeLineIndex;

            } else
            {
                for (int i = lineBufferVertices.size() - 1; i >= edge->m_EdgeLineIndex; --i)
                {
                    lineBufferVertices.erase(lineBufferVertices.begin() + i);
                }
                
            }


        }

	}
};
