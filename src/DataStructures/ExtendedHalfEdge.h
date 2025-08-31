#pragma once
#include "HalfEdge.h"

class ExtendedHalfEdge;
class ExtendedEdge;
class ExtendedVertex;
class ExtendedFace;
class ExtendedHalfEdgeMesh;


struct ExtendedHalfEdgeTraits {
    using FType = ExtendedFace;   
    using EType = ExtendedEdge;
    using HType = ExtendedHalfEdge;
    using VType = ExtendedVertex;
    using DerType = ExtendedHalfEdgeMesh;
};

class GraphEdge
{
public:
    ExtendedVertex* vertex = nullptr;
    int graphEdgeIndexInVertex = -1;
    ExtendedFace* face = nullptr;
    int graphEdgeIndexInFace = -1;
};

struct FaceTriangle
{
    int indexInVAO = -1;
    ExtendedFace* face = nullptr;
    int indexInFace = -1;
};


class ExtendedFace : public HalfEdgeDS::Face<ExtendedHalfEdgeTraits> {
public:
    ExtendedFace(ExtendedHalfEdgeMesh& mesh) : ExtendedFace::Face(mesh) {
    }
    
    //graph
    std::vector<GraphEdge*> m_graphEdges;

    //selection
    bool m_selected = false;
    int m_selectionIndex = -1;

    //material
    Material* material = nullptr;
    std::vector<FaceTriangleIndex> faceTriangleIndices;
};

class ExtendedEdge : public HalfEdgeDS::Edge<ExtendedHalfEdgeTraits> {
public:
    ExtendedEdge(ExtendedHalfEdgeMesh& mesh) : ExtendedEdge::Edge(mesh) {
    }

    EdgeLineIndex m_EdgeLineIndex = -1;
};

class ExtendedHalfEdge : public HalfEdgeDS::HalfEdge<ExtendedHalfEdgeTraits> {
public:
    ExtendedHalfEdge(ExtendedHalfEdgeMesh& mesh) : ExtendedHalfEdge::HalfEdge(mesh) {
    }
};

class ExtendedVertex : public HalfEdgeDS::Vertex<ExtendedHalfEdgeTraits> {
public:
    ExtendedVertex(ExtendedHalfEdgeMesh& mesh) : ExtendedVertex::Vertex(mesh) {
    }

    //graph
    std::vector<GraphEdge*> m_graphEdges;

    std::vector<ExtendedEdge*> m_neighbourEdges;
};

class ExtendedHalfEdgeMesh : public HalfEdgeDS::HalfEdgeMesh<ExtendedHalfEdgeTraits> {
public:
    ExtendedHalfEdgeMesh() : ExtendedHalfEdgeMesh::HalfEdgeMesh() {
    }

    void build(const std::vector<std::vector<int>>& polygons, const std::vector<glm::vec3>& vertices){
        ExtendedHalfEdgeMesh::HalfEdgeMesh::build(polygons, vertices);
        buildGraph();
    }

private:

    void buildGraph()
    {
        //---GO_THROUGH_ALL_EDGES---
        for (ExtendedEdge* edge : m_edges) {
            ExtendedVertex* firstVertex = edge->m_firstVertex;
            ExtendedVertex* secondVertex = edge->m_secondVertex;
            firstVertex->m_neighbourEdges.emplace_back(edge);
            secondVertex->m_neighbourEdges.emplace_back(edge);
        } 

        //---GO_THROUGH_ALL_FACES---
        for (ExtendedFace* face : m_faces)
        {
            ExtendedHalfEdge* halfEdge = face->m_halfEdge;
            do {
                //---ACCESS_EACH_VERTEX_OF_FACE---
                ExtendedVertex* vertex = halfEdge->m_vertex;
                halfEdge = halfEdge->m_next;

                //---CREATE_GRAPH_EDGE---
                GraphEdge* graphEdge = new GraphEdge();
                graphEdge->vertex = vertex;
                graphEdge->graphEdgeIndexInVertex = vertex->m_graphEdges.size();

                graphEdge->face = face;
                graphEdge->graphEdgeIndexInFace = face->m_graphEdges.size();

                vertex->m_graphEdges.emplace_back(graphEdge);
                face->m_graphEdges.emplace_back(graphEdge);

            } while (halfEdge != face->m_halfEdge);
        }

        std::cout << "Graph built" << std::endl;
    }

public:
    std::map<Material*, std::vector<FaceTriangle>> m_faceTriangles;
};


