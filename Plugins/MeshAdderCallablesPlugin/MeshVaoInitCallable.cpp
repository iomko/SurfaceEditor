#include "MeshVaoInitCallable.h"
#include "../../src/Callables/CallableRegistry.h"
#include "../../src/Utils/GeometryUtils.h"
#include "../../src/Structures/ExtendedHalfEdge.h"
// #include "Renderer/Renderer.h"
#include "../../src/Renderer/BufferStorages.h"
#include "../../src/Renderer/MaterialRegistry.h"

static AutoRegisterCallable<MeshVaoInitCallable> reg("MESH_VAO_INIT_CALLABLE");

void MeshVaoInitCallable::invoke(const MeshParams &input)
{
    Mesh *mesh = input.m_mesh;
    Material *defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");
    Material *defaultMeshMaterial = MaterialRegistry::getMaterial("defaultMeshMaterial");

    std::vector<ExtendedFace *> &meshFaces = input.m_mesh->getHalfEdgeStructure()->m_faces;
    std::vector<ExtendedEdge *> &meshEdges = input.m_mesh->getHalfEdgeStructure()->m_edges;

    std::vector<FaceTriangle> &halfEdgeFaceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles[defaultMeshMaterial];
    // Register Mesh BufferStorage
    //
    TriangleBufferStorage &triangleBufferStorage = mesh->bufferLayout.registerTriangleBufferStorage(defaultMeshMaterial);
    triangleBufferStorage.create();
    // MeshBufferStorage* meshBufferStorage = Renderer::s_bufferRegistry.queryBuffer<MeshBufferStorage>();
    // meshBufferStorage->registerBufferStorage(mesh, material);

    // Get Raw Mesh Buffer Data For Specified Mesh
    //
    std::vector<BufferStorageDataType::TriangleVertex> &triangleBufferVertices = triangleBufferStorage.data.vertices;

    // Register Line BufferStorage
    //
    LineBufferStorage &lineBufferStorage = mesh->bufferLayout.registerLineBufferStorage(defaultLineMaterial);
    lineBufferStorage.create();

    // Get Raw Line Buffer Data For Specified Mesh
    //
    std::vector<BufferStorageDataType::LineVertex> &lineBufferVertices = lineBufferStorage.data.vertices;

    for (ExtendedFace *meshFace : meshFaces)
    {
        meshFace->material = defaultMeshMaterial;
        glm::vec3 faceNormal = utils::geometry::computePolygonNormal(meshFace);

        // Triangulation
        std::vector<glm::vec3> faceVertices;
        for (auto it = meshFace->faceVertexBegin(); it != meshFace->faceVertexEnd(); ++it)
        {
            glm::vec3 vertexPosition = (*it).m_position;
            faceVertices.emplace_back(vertexPosition);
        }
        glm::vec3 firstVertex = faceVertices.at(0);
        faceVertices.emplace_back(firstVertex);

        std::vector<glm::vec3> triangulatedVertices = utils::geometry::triangulatePolygon(faceVertices);

        // Update ExtendedFace info
        for (int i = 0, indexInFace = 0; i < triangulatedVertices.size(); i += 3, ++indexInFace)
        {
            triangleBufferVertices.emplace_back(triangulatedVertices.at(i), faceNormal, false);
            triangleBufferVertices.emplace_back(triangulatedVertices.at(i + 1), faceNormal, false);
            triangleBufferVertices.emplace_back(triangulatedVertices.at(i + 2), faceNormal, false);

            FaceTriangle faceTriangle;
            faceTriangle.face = meshFace;
            faceTriangle.indexInVAO = triangleBufferVertices.size() - 3;
            faceTriangle.indexInFace = indexInFace;

            halfEdgeFaceTriangles.emplace_back(faceTriangle);

            int faceTriangleIndex = halfEdgeFaceTriangles.size() - 1;
            meshFace->faceTriangleIndices.emplace_back(faceTriangleIndex);
        }
    }

    // Update ExtendedEdge info
    for (ExtendedEdge *edge : meshEdges)
    {
        ExtendedVertex *firstVertex = edge->m_firstVertex;
        ExtendedVertex *secondVertex = edge->m_secondVertex;

        ExtendedHalfEdge *halfEdge = edge->m_halfEdge;
        glm::vec3 normal = utils::geometry::computePolygonNormal(halfEdge->m_face);

        if (halfEdge->m_twin != nullptr)
        {
            glm::vec3 twinNormal = utils::geometry::computePolygonNormal(halfEdge->m_twin->m_face);
            normal = glm::normalize(normal + twinNormal);
        }

        lineBufferVertices.emplace_back(firstVertex->m_position, false);
        lineBufferVertices.emplace_back(secondVertex->m_position, false);

        int edgeLineIndex = lineBufferVertices.size() - 2;
        edge->m_EdgeLineIndex = edgeLineIndex;
    }

    triangleBufferStorage.update();
    lineBufferStorage.update();
    // meshBufferStorage->updateBufferStorage(mesh, material);
    // lineBufferStorage->updateBufferStorage(mesh);
}