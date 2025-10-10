#pragma once

#include "../Params/OperationParams.h"
#include "Callables/Callable.h"
#include "../Utils/GeometryUtils.h"
#include "../Renderer/Renderer.h"
#include "../Renderer/MaterialRegistry.h"

class EdgesVaoInitCallable : public Callable<EdgeParams, void>
{
public:
	void invoke(const EdgeParams& input) override
	{
        Mesh* inputMesh = input.mesh;
        std::vector<ExtendedEdge*>& inputEdges = *(input.edges);
   
        //Get Raw Line Buffer Data For Specified Mesh
        //
        //

        Material* defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");

        if (auto opt = inputMesh->bufferLayout.getLineBufferStorage(defaultLineMaterial)) {

            LineBufferStorage& lineBufferStorage = opt->get();
            std::vector<BufferStorageDataType::LineVertex>& lineBufferVertices = lineBufferStorage.data.vertices;


            for(ExtendedEdge* edge : inputEdges) {
                ExtendedVertex* firstVertex = edge->m_firstVertex;
                ExtendedVertex* secondVertex = edge->m_secondVertex;
                    
                lineBufferVertices.emplace_back(firstVertex->m_position, false);
                lineBufferVertices.emplace_back(secondVertex->m_position, false);

                //Inject info about the position of line inside of it's buffer to the ExtendedEdge class
                int edgeLineIndex = lineBufferVertices.size() - 2;
                edge->m_EdgeLineIndex = edgeLineIndex;
            }
            
            //Notify buffer storage about the new changes being made to it
            lineBufferStorage.update();

        }

    }

};



