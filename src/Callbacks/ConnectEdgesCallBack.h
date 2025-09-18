#pragma once
#include "Params/OperationParams.h"
#include "Callbacks/Callback.h"
#include "Patterns/Observer.h"
#include "Utils/ContainerUtils.h"
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
        //potrebujem ziskat meshLinesVaoVector
        LineBufferStorage* lineBufferStorage = Renderer::s_bufferRegistry.queryBuffer<LineBufferStorage>();
        LineBufferStorage::MeshBuffMap& meshLinesMap = lineBufferStorage->meshBuffMap;
        BufferData<RendererBuffersData::LineVertex>* lineBufferData;
        lineBufferStorage->getBufferData(mesh, lineBufferData);
        std::vector<RendererBuffersData::LineVertex>& meshLinesVaoVector = lineBufferData->vertices;


		//---VAO_DATA_SWAP---
		//before
		if (edge->m_EdgeLineIndex != (meshLinesVaoVector.size() - 2))
		{
			utils::containers::reverseSubrange(meshLinesVaoVector, meshLinesVaoVector.size() - 2, meshLinesVaoVector.size() - 1);
			for (int i = edge->m_EdgeLineIndex; i <= edge->m_EdgeLineIndex + 1; ++i)
			{
				utils::containers::swapLastAndPop(meshLinesVaoVector, i);
			}

			mesh->m_halfEdgeStructure->m_edges.back()->m_EdgeLineIndex = edge->m_EdgeLineIndex;

		} else
		{
			for (int i = meshLinesVaoVector.size() - 1; i >= edge->m_EdgeLineIndex; --i)
			{
				meshLinesVaoVector.erase(meshLinesVaoVector.begin() + i);
			}

			if(meshLinesVaoVector.empty())
			{
                meshLinesMap.erase(mesh);
			}

		}

	}
};
