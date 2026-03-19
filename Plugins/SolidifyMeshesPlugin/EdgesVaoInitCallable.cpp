#include "EdgesVaoInitCallable.h"
#include "../../src/Callables/CallableRegistry.h"
#include "../../src/Utils/GeometryUtils.h"
#include "../../src/Renderer/Renderer.h"
#include "../../src/Renderer/MaterialRegistry.h"

static AutoRegisterCallable<EdgesVaoInitCallable> reg("EDGES_VAO_INIT_CALLABLE");

void EdgesVaoInitCallable::invoke(const EdgeParams &input)
{
    Mesh *inputMesh = input.mesh;
    std::vector<ExtendedEdge *> &inputEdges = *(input.edges);

    // Get Raw Line Buffer Data For Specified Mesh
    //
    //

    Material *defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");

    if (auto opt = inputMesh->bufferLayout.getLineBufferStorage(defaultLineMaterial))
    {

        LineBufferStorage &lineBufferStorage = opt->get();
        std::vector<BufferStorageDataType::LineVertex> &lineBufferVertices = lineBufferStorage.data.vertices;

        for (ExtendedEdge *edge : inputEdges)
        {
            ExtendedVertex *firstVertex = edge->m_firstVertex;
            ExtendedVertex *secondVertex = edge->m_secondVertex;

            lineBufferVertices.emplace_back(firstVertex->m_position, false);
            lineBufferVertices.emplace_back(secondVertex->m_position, false);

            // Inject info about the position of line inside of it's buffer to the ExtendedEdge class
            int edgeLineIndex = lineBufferVertices.size() - 2;
            edge->m_EdgeLineIndex = edgeLineIndex;
        }

        // Notify buffer storage about the new changes being made to it
        lineBufferStorage.update();
    }
}