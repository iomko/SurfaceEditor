#include "DeleteFaceCallBack.h"
#include "CallbackRegister.h"
#include "../Utils/ContainerUtils.h"
#include "../Structures/ExtendedHalfEdge.h"
#include "../ViewPortsController.h"
#include "../Renderer/MaterialRegistry.h"

static AutoRegisterCallback<DeleteFaceCallBack> registerDeleteFaceCallBack;

void DeleteFaceCallBack::execute(const SingleFaceParams &iParams)
{
    Mesh *selectedMesh = iParams.mesh;
    ExtendedFace *selectedFace = iParams.face;
    Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

    // first delete face from octrees
    scene->deleteFaceFromOctrees(selectedMesh, selectedFace);

    // delete face data from triangleBufferStorage
    // so the deleted face wont be drawn
    deleteTriangleBufferData(selectedMesh, selectedFace);

    // delete face data from extended halfedge structure
    deleteHalfEdgeStructureData(selectedMesh, selectedFace);

    // selectedMesh->calculateMeshBounds();
}
void DeleteFaceCallBack::deleteHalfEdgeStructureData(Mesh *mesh, ExtendedFace *face)
{
    ExtendedHalfEdgeMesh *halfEdgeMesh = mesh->m_halfEdgeStructure;

    // Go Through all vertices of a face
    ExtendedHalfEdge *halfEdge = face->m_halfEdge;
    do
    {
        // do something with the vertex Vertex*
        // and the halfEdge HalfEdge*
        ExtendedVertex *vertex = halfEdge->m_vertex;
        ExtendedHalfEdge *nextHalfEdge = halfEdge->m_next;

        // potrebujem vymazat graphEdge z m_graphEdges

        // najskor z vertexu
        GraphEdge *foundGraphEdge = nullptr;
        for (GraphEdge *graphEdge : vertex->m_graphEdges)
        {
            if (graphEdge->face == face)
            {
                foundGraphEdge = graphEdge;
                break;
            }
        }

        // Delete GraphEdge
        if (foundGraphEdge->graphEdgeIndexInVertex != (vertex->m_graphEdges.size() - 1))
        {
            vertex->m_graphEdges.back()->graphEdgeIndexInVertex = foundGraphEdge->graphEdgeIndexInVertex;

            utils::containers::swapLastAndPop(vertex->m_graphEdges, foundGraphEdge->graphEdgeIndexInVertex);
        }
        else
        {
            vertex->m_graphEdges.pop_back();
        }

        if (foundGraphEdge->graphEdgeIndexInFace != (face->m_graphEdges.size() - 1))
        {
            face->m_graphEdges.back()->graphEdgeIndexInFace = foundGraphEdge->graphEdgeIndexInFace;

            utils::containers::swapLastAndPop(face->m_graphEdges, foundGraphEdge->graphEdgeIndexInFace);
        }
        else
        {
            face->m_graphEdges.pop_back();
        }

        // Check if vertex needs to be deleted from HalfEdge
        if (vertex->m_graphEdges.empty())
        {
            if (vertex->m_vertexIndexInVector != (halfEdgeMesh->m_vertices.size() - 1))
            {
                halfEdgeMesh->m_vertices.back()->m_vertexIndexInVector = vertex->m_vertexIndexInVector;
                utils::containers::swapLastAndPop(halfEdgeMesh->m_vertices, vertex->m_vertexIndexInVector);
            }
            else
            {
                halfEdgeMesh->m_vertices.pop_back();
            }

            vertex->m_halfEdge->m_vertex = nullptr;
        }
        else
        {
            // musime najst novy halfEdge ktory bude pripadat vertexu
            ExtendedHalfEdge *neighborFaceHalfEdge = vertex->m_graphEdges.front()->face->m_halfEdge;

            while (neighborFaceHalfEdge->m_vertex != vertex)
            {
                ExtendedHalfEdge *neighborFaceNextHalfEdge = neighborFaceHalfEdge->m_next;
                neighborFaceHalfEdge = neighborFaceNextHalfEdge;
            }

            vertex->m_halfEdge = neighborFaceHalfEdge;
        }

        // Edge deletion
        if (halfEdge->m_twin == nullptr)
        {
            deleteLineBufferData(mesh, halfEdge->m_edge);

            if (halfEdge->m_edge->m_edgeIndexInVector != (halfEdgeMesh->m_edges.size() - 1))
            {
                halfEdgeMesh->m_edges.back()->m_edgeIndexInVector = halfEdge->m_edge->m_edgeIndexInVector;
                utils::containers::swapLastAndPop(halfEdgeMesh->m_edges, halfEdge->m_edge->m_edgeIndexInVector);
            }
            else
            {
                halfEdgeMesh->m_edges.pop_back();
            }
        }
        else
        {
            halfEdge->m_edge->m_isOuter = true;

            if (halfEdge->m_edge->m_halfEdge == halfEdge)
            {
                halfEdge->m_edge->m_halfEdge = halfEdge->m_twin;
            }

            halfEdge->m_twin->m_twin = nullptr;
        }

        // HalfEdge deletion
        if (halfEdge->m_halfEdgeIndexInVector != (halfEdgeMesh->m_halfEdges.size() - 1))
        {
            halfEdgeMesh->m_halfEdges.back()->m_halfEdgeIndexInVector = halfEdge->m_halfEdgeIndexInVector;
            utils::containers::swapLastAndPop(halfEdgeMesh->m_halfEdges, halfEdge->m_halfEdgeIndexInVector);
        }
        else
        {
            halfEdgeMesh->m_halfEdges.pop_back();
        }

        // Go next
        halfEdge = nextHalfEdge;
    } while (halfEdge != face->m_halfEdge);

    // At the end delete face
    if (face->m_faceIndexInVector != (halfEdgeMesh->m_faces.size() - 1))
    {
        halfEdgeMesh->m_faces.back()->m_faceIndexInVector = face->m_faceIndexInVector;
        utils::containers::swapLastAndPop(halfEdgeMesh->m_faces, face->m_faceIndexInVector);
    }
    else
    {
        halfEdgeMesh->m_faces.pop_back();
    }
}
void DeleteFaceCallBack::deleteLineBufferData(Mesh *mesh, ExtendedEdge *edge)
{

    Material *defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");
    if (auto opt = mesh->bufferLayout.getLineBufferStorage(defaultLineMaterial))
    {

        LineBufferStorage &lineBufferStorage = opt->get();
        std::vector<BufferStorageDataType::LineVertex> &lineBufferVertices = lineBufferStorage.data.vertices;

        if (edge->m_EdgeLineIndex != (lineBufferVertices.size() - 2))
        {
            utils::containers::reverseSubrange(lineBufferVertices, lineBufferVertices.size() - 2, lineBufferVertices.size() - 1);
            for (int i = edge->m_EdgeLineIndex; i <= edge->m_EdgeLineIndex + 1; ++i)
            {
                utils::containers::swapLastAndPop(lineBufferVertices, i);
            }

            mesh->m_halfEdgeStructure->m_edges.back()->m_EdgeLineIndex = edge->m_EdgeLineIndex;
        }
        else
        {
            for (int i = lineBufferVertices.size() - 1; i >= edge->m_EdgeLineIndex; --i)
            {
                lineBufferVertices.erase(lineBufferVertices.begin() + i);
            }
        }

        for (auto it = mesh->bufferLayout.lineBuffersBegin(); it != mesh->bufferLayout.lineBuffersEnd(); ++it)
        {
            LineBufferStorage &lineBufferStorage = it->second;
            lineBufferStorage.update();
        }
    }
}
void DeleteFaceCallBack::deleteTriangleBufferData(Mesh *mesh, ExtendedFace *face)
{

    Material *faceMaterial = MaterialRegistry::getMaterial(face->material->getName());
    if (auto opt = mesh->bufferLayout.getTriangleBufferStorage(faceMaterial))
    {

        TriangleBufferStorage &triangleBufferStorage = opt->get();
        std::vector<BufferStorageDataType::TriangleVertex> &triangleBufferVertices = triangleBufferStorage.data.vertices;

        std::map<Material *, std::vector<FaceTriangle>> &materialTrianglesMap = mesh->m_halfEdgeStructure->m_faceTriangles;
        auto materialTrianglesIt = materialTrianglesMap.find(faceMaterial);
        std::vector<FaceTriangle> &faceTriangles = materialTrianglesIt->second;

        while (!face->faceTriangleIndices.empty())
        {
            FaceTriangleIndex delFaceTriangleIndex = face->faceTriangleIndices.front();
            FaceTriangle &delFaceTriangle = faceTriangles.at(delFaceTriangleIndex);

            // deletion inside of TrianglesBufferStorage
            if (delFaceTriangleIndex != faceTriangles.size() - 1)
            {
                faceTriangles.back().indexInVAO = delFaceTriangle.indexInVAO;

                utils::containers::reverseSubrange(triangleBufferVertices, triangleBufferVertices.size() - 3, triangleBufferVertices.size() - 1);
                for (int i = delFaceTriangle.indexInVAO; i <= delFaceTriangle.indexInVAO + 2; ++i)
                {
                    utils::containers::swapLastAndPop(triangleBufferVertices, i);
                }
            }
            else
            {
                for (int i = triangleBufferVertices.size() - 1; i >= delFaceTriangle.indexInVAO; --i)
                {
                    triangleBufferVertices.erase(triangleBufferVertices.begin() + i);
                }

                if (triangleBufferVertices.empty())
                {
                    mesh->bufferLayout.m_matTriangleBuff.erase(faceMaterial);

                    if (mesh->bufferLayout.m_matTriangleBuff.empty())
                    {
                        // tak potom delete mesh
                    }
                }
            }

            // deletion inside of extended halfedge structure
            ExtendedFace *lastTriangleFace = faceTriangles.back().face;
            lastTriangleFace->faceTriangleIndices.at(faceTriangles.back().indexInFace) = delFaceTriangleIndex;

            utils::containers::swapLastAndPop(faceTriangles, delFaceTriangleIndex);

            if (faceTriangles.empty())
            {
                materialTrianglesMap.erase(materialTrianglesIt);
            }

            if (face->faceTriangleIndices.size() != 1)
            {
                faceTriangles.at(face->faceTriangleIndices.back()).indexInFace = 0;
            }

            utils::containers::swapLastAndPop(face->faceTriangleIndices, 0);
        }

        for (auto it = mesh->bufferLayout.triangleBuffersBegin(); it != mesh->bufferLayout.triangleBuffersEnd(); ++it)
        {
            TriangleBufferStorage &triangleBufferStorage = it->second;
            triangleBufferStorage.update();
        }
    }
}