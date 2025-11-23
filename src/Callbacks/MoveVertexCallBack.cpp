#include "MoveVertexCallBack.h"
#include "CallbackRegister.h"
#include "../ViewPortsController.h"
#include "../Utils/InterpolationUtils.h"
#include "../Utils/GeometryUtils.h"
#include "../Renderer/MaterialRegistry.h"

static AutoRegisterCallback<MoveVertexCallBack> moveVertexCallBackReg;
void MoveVertexCallBack::execute(const VertexParams &iParams)
{
    const float epsilon = 0.001f;

    ExtendedVertex *vertex = iParams.vertex;
    Mesh *mesh = iParams.mesh;
    glm::vec3 moveByVector = iParams.newPosition;

    Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

    if (vertex != nullptr)
    {
        // get lineBufferVertices
        // updated lineBufferStorage
        Material *defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");
        if (auto opt = mesh->bufferLayout.getLineBufferStorage(defaultLineMaterial))
        {

            LineBufferStorage &lineBufferStorage = opt->get();
            std::vector<BufferStorageDataType::LineVertex> &lineBufferVertices = lineBufferStorage.data.vertices;

            for (ExtendedEdge *edge : vertex->m_neighbourEdges)
            {
                int edgeIndexInVao = edge->m_EdgeLineIndex;

                if (glm::all(glm::epsilonEqual(edge->m_firstVertex->m_position, vertex->m_position, epsilon)))
                {
                    lineBufferVertices.at(edgeIndexInVao).position += moveByVector;
                    // lineBufferVertices.at(edgeIndexInVao).position += (normal * scalingFactor);
                }
                else if (glm::all(glm::epsilonEqual(edge->m_secondVertex->m_position, vertex->m_position, epsilon)))
                {
                    lineBufferVertices.at(edgeIndexInVao + 1).position += moveByVector;
                    // lineBufferVertices.at(edgeIndexInVao + 1).position += (normal * scalingFactor);
                }
            }

            lineBufferStorage.update();
        }

        for (GraphEdge *graphEdge : vertex->m_graphEdges)
        {
            ExtendedFace *face = graphEdge->face;
            FaceTriangleIndex faceTriangleIndex = face->faceTriangleIndices.front();
            FaceTriangle &faceTriangle = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second.at(faceTriangleIndex);

            Material *faceMaterial = MaterialRegistry::getMaterial(face->material->getName());
            if (auto opt = mesh->bufferLayout.getTriangleBufferStorage(faceMaterial))
            {

                TriangleBufferStorage &triangleBufferStorage = opt->get();
                std::vector<BufferStorageDataType::TriangleVertex> &triangleBufferVertices = triangleBufferStorage.data.vertices;

                int faceIndexInVao = faceTriangle.indexInVAO;
                for (int i = faceIndexInVao; i < faceIndexInVao + 3; ++i)
                {
                    if (glm::all(glm::epsilonEqual(triangleBufferVertices.at(i).position, vertex->m_position, epsilon)))
                    {
                        triangleBufferVertices.at(i).position += moveByVector;
                        break;
                    }
                }

                scene->deleteFaceFromOctrees(mesh, face);
                scene->addFaceIntoOctrees(mesh, face);
            }
        }

        vertex->m_position += moveByVector;

        for (auto it = mesh->bufferLayout.triangleBuffersBegin(); it != mesh->bufferLayout.triangleBuffersEnd(); ++it)
        {
            TriangleBufferStorage &triangleBufferStorage = it->second;
            triangleBufferStorage.update();
        }
    }
}
