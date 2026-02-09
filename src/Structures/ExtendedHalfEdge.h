#pragma once
#include "HalfEdge.h"
#include "Utils/ContainerUtils.h"
#include <algorithm>

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

    //skewed
    bool m_isSkewed = false;
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

public:
    ExtendedFace* addFloatingFace(const std::vector<glm::vec3>& vertices){
        
        bool firstHalfEdge = true;
        ExtendedFace* face = this->createObject<ExtendedFace>(this->m_faces);

        std::map<glm::vec3, ExtendedVertex*> helperVerticesMap;
        std::map<std::pair<glm::vec3, glm::vec3>, ExtendedHalfEdge*> helperHalfEdgesMap;

        for (int vertexIndex = 0; vertexIndex < vertices.size(); ++vertexIndex) {
            glm::vec3 firstVertexCoords = vertices.at(vertexIndex);
            glm::vec3 secondVertexCoords = vertices.at((vertexIndex+1) % vertices.size());
            
            bool firstVertexCreated = false;
            bool secondVertexCreated = false;
            ExtendedVertex* firstVertex = this->findOrCreateObject<ExtendedVertex>(firstVertexCoords, helperVerticesMap, this->m_vertices, &firstVertexCreated);
            if (firstVertexCreated) firstVertex->m_vertexIndexInVector = this->m_vertices.size() - 1;
            ExtendedVertex* secondVertex = this->findOrCreateObject<ExtendedVertex>(secondVertexCoords, helperVerticesMap, this->m_vertices, &secondVertexCreated);
            if (secondVertexCreated) secondVertex->m_vertexIndexInVector = this->m_vertices.size() - 1;

            ExtendedEdge* edge = this->createObject<ExtendedEdge>(this->m_edges);

            bool newHalfEdgeCreated = false;
            ExtendedHalfEdge* currentHalfEdge = this->findOrCreateObject<ExtendedHalfEdge>(std::make_pair(firstVertexCoords, secondVertexCoords), helperHalfEdgesMap, this->m_halfEdges, &newHalfEdgeCreated);
              
            //set current half edge
            //
            //currentHalfEdge->m_previous = previousHalfEdge;
            currentHalfEdge->m_vertex = firstVertex;
            currentHalfEdge->m_edge = edge;
            currentHalfEdge->m_face = face;
            if (newHalfEdgeCreated) currentHalfEdge->m_halfEdgeIndexInVector = this->m_halfEdges.size() - 1;

            ExtendedHalfEdge* previousHalfEdge = this->findOrCreateObject<ExtendedHalfEdge>(std::make_pair(vertices.at((vertexIndex+(vertices.size()-1)) % vertices.size()), firstVertexCoords), helperHalfEdgesMap, this->m_halfEdges, &newHalfEdgeCreated);  
            //set current half edge
            //
            currentHalfEdge->m_previous = previousHalfEdge;

            if (newHalfEdgeCreated) previousHalfEdge->m_halfEdgeIndexInVector = this->m_halfEdges.size() - 1;

            ExtendedHalfEdge* nextHalfEdge = this->findOrCreateObject<ExtendedHalfEdge>(std::make_pair(secondVertexCoords, vertices.at((vertexIndex+2) % vertices.size())), helperHalfEdgesMap, this->m_halfEdges, &newHalfEdgeCreated);
           
            currentHalfEdge->m_next = nextHalfEdge;
            if (newHalfEdgeCreated) nextHalfEdge->m_halfEdgeIndexInVector = this->m_halfEdges.size() - 1;
            
            //set edge
            //
            edge->m_halfEdge = currentHalfEdge;
            edge->m_firstVertex = firstVertex;
            edge->m_secondVertex = secondVertex;
            edge->m_edgeIndexInVector = this->m_edges.size() - 1;
            edge->m_isOuter = true;

            //set vertex

            
            if (!firstVertex->m_halfEdge) { firstVertex->m_halfEdge = currentHalfEdge, firstVertex->m_position = firstVertexCoords; }
            
            //set face
            if (!face->m_halfEdge) { face->m_faceIndexInVector = this->m_faces.size() - 1, face->m_halfEdge = currentHalfEdge; }
        }

        //build graph for that face
        buildGraphForFace(face);

        return face;
    }

    void connectEdges(ExtendedEdge* firstEdge, ExtendedEdge* secondEdge){
        
        ExtendedHalfEdge* firstHalfEdge = firstEdge->m_halfEdge;
        ExtendedHalfEdge* secondHalfEdge = secondEdge->m_halfEdge;

        ExtendedHalfEdgeMesh& mesh = firstEdge->m_mesh;

        ExtendedVertex* firstVertex = firstHalfEdge->m_vertex;
        ExtendedVertex* secondVertex = firstHalfEdge->m_next->m_vertex;

        if(firstEdge == secondEdge) return;

        if(firstHalfEdge->m_vertex->m_position == secondHalfEdge->m_next->m_vertex->m_position &&
            firstHalfEdge->m_next->m_vertex->m_position == secondHalfEdge->m_vertex->m_position) 
        {   
            std::vector<std::pair<ExtendedVertex*, ExtendedVertex*>> points{ 
                std::make_pair(firstHalfEdge->m_vertex, secondHalfEdge->m_next->m_vertex),
                std::make_pair(firstHalfEdge->m_next->m_vertex, secondHalfEdge->m_vertex)
            };

            for (std::pair<ExtendedVertex*, ExtendedVertex*>& point : points) {

                ExtendedVertex* vertex = point.first;
                ExtendedVertex* oppositeVertex = point.second;

                std::vector<ExtendedEdge*>& edges = vertex->m_neighbourEdges;

                for (ExtendedEdge* edge : edges) {
                    
                    if(edge->m_halfEdge->m_vertex == vertex) {
                        edge->m_halfEdge->m_vertex = oppositeVertex;
                    }

                    if(edge->m_halfEdge->m_twin != nullptr && edge->m_halfEdge->m_twin->m_vertex == vertex) {
                        edge->m_halfEdge->m_twin->m_vertex = oppositeVertex;
                    }

                    if(edge->m_firstVertex->m_position == vertex->m_position) {
                        edge->m_firstVertex = oppositeVertex;
                    } else if(edge->m_secondVertex->m_position == vertex->m_position) {
                        edge->m_secondVertex = oppositeVertex;
                    }

                }

                std::vector<ExtendedVertex*>& meshVertices = mesh.m_vertices;
                
                //prehod neighbour edges
                //musime vymazat jeden edge
                auto it = std::find(vertex->m_neighbourEdges.begin(), vertex->m_neighbourEdges.end(), firstEdge);
                if(it != vertex->m_neighbourEdges.end()){
                    vertex->m_neighbourEdges.erase(it);
                }
                oppositeVertex->m_neighbourEdges.insert(
                        oppositeVertex->m_neighbourEdges.end(),
                        vertex->m_neighbourEdges.begin(),
                        vertex->m_neighbourEdges.end()
                        );

                ExtendedVertex*& lastVertex = meshVertices.back();
                lastVertex->m_vertexIndexInVector = vertex->m_vertexIndexInVector;
                utils::containers::swapLastAndPop(meshVertices, vertex->m_vertexIndexInVector);
            }

            //halfEdge
            firstEdge->m_halfEdge->m_edge = secondEdge;
            //twins
            secondEdge->m_halfEdge->m_twin = firstEdge->m_halfEdge;
            firstEdge->m_halfEdge->m_twin = secondEdge->m_halfEdge;


            std::vector<ExtendedEdge*>& meshEdges = mesh.m_edges;
            ExtendedEdge*& lastEdge = meshEdges.back();

            lastEdge->m_edgeIndexInVector = firstEdge->m_edgeIndexInVector;
            utils::containers::swapLastAndPop(meshEdges, firstEdge->m_edgeIndexInVector);

        }

    }

private:
    void buildGraphForFace(ExtendedFace* face){
                //---GO_THROUGH_ALL_EDGES---
        for(auto it = face->faceHalfEdgeBegin(); it != face->faceHalfEdgeEnd(); ++it){
            ExtendedEdge* edge = (*it).m_edge;
            ExtendedVertex* firstVertex = edge->m_firstVertex;
            ExtendedVertex* secondVertex = edge->m_secondVertex;

            firstVertex->m_neighbourEdges.emplace_back(edge);
            secondVertex->m_neighbourEdges.emplace_back(edge);



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
    }

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


