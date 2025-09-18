
#include "../Params/OperationParams.h"
#include "Callables/Callable.h"
#include "../Utils/GeometryUtils.h"
#include "../Renderer/Renderer.h"

class EdgesVaoInitCallable : public Callable<EdgeParams, void>
{
public:
	void invoke(const EdgeParams& input) override
	{
        Mesh* mesh = input.mesh;
        std::vector<ExtendedEdge*>& edges = *(input.edges);
    
        //potrebujem ziskat meshLinesVaoVector
        LineBufferStorage* lineBufferStorage = Renderer::s_bufferRegistry.queryBuffer<LineBufferStorage>();
        BufferData<RendererBuffersData::LineVertex>* lineBufferData;
        lineBufferStorage->getBufferData(mesh, lineBufferData);
        std::vector<RendererBuffersData::LineVertex>& meshLinesVaoVector = lineBufferData->vertices;

        for(ExtendedEdge* edge : edges) {
            ExtendedVertex* edgeFirstVertex = edge->m_firstVertex;
            ExtendedVertex* edgeSecondVertex = edge->m_secondVertex;
                
            meshLinesVaoVector.emplace_back(edgeFirstVertex->m_position, false);
            meshLinesVaoVector.emplace_back(edgeSecondVertex->m_position, false);

            //---ADD_INFO_INTO_EDGE---
            int edgeLineIndex = meshLinesVaoVector.size() - 2;
            edge->m_EdgeLineIndex = edgeLineIndex;
        }

        lineBufferStorage->updateBufferStorage(mesh);
    }

};



