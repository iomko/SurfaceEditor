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
        //Get Raw Line Buffer Data For Specified Mesh
        LineBufferStorage* lineBufferStorage = Renderer::s_bufferRegistry.queryBuffer<LineBufferStorage>();
        LineBufferStorage::MeshBuffMap& meshLinesMap = lineBufferStorage->meshBuffMap;
        BufferData<RendererBuffersData::LineVertex>* lineBufferData;
        lineBufferStorage->getBufferData(mesh, lineBufferData);
        std::vector<RendererBuffersData::LineVertex>& lineBufferVertices = lineBufferData->vertices;

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

			if(lineBufferVertices.empty())
			{
                meshLinesMap.erase(mesh);
			}
		}

	}
};
