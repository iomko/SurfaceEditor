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

    //gizmo
    glm::mat4 m_transform = glm::mat4(1.0f);
    glm::mat4* m_realTimeTransform = nullptr;
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

   std::tuple<ExtendedEdge*, ExtendedEdge*, ExtendedEdge*, ExtendedEdge*> splitEdge(ExtendedEdge* edge) {

        std::tuple<ExtendedEdge*, ExtendedEdge*, ExtendedEdge*, ExtendedEdge*> resTuple{nullptr, nullptr, nullptr, nullptr};
        //sucastou prveho facu
        ExtendedEdge* currEdge = edge;
        std::get<0>(resTuple) = currEdge;
        ExtendedHalfEdge* currHalfEdge = currEdge->m_halfEdge;

        ExtendedVertex* secondFaceVertex = currHalfEdge->m_vertex;
        ExtendedHalfEdge* secondFaceHalfEdge = currHalfEdge->m_next->m_next;

        ExtendedHalfEdge* nextHalfEdge = currHalfEdge->m_next;
        auto* newSplitHalfEdge = createObject<typename ExtendedHalfEdgeTraits::HType>(m_halfEdges);
        newSplitHalfEdge->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

        auto* newSplitHalfEdgeTwin = createObject<typename ExtendedHalfEdgeTraits::HType>(m_halfEdges);
        newSplitHalfEdgeTwin->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

        auto* newSplitEdge = createObject<typename ExtendedHalfEdgeTraits::EType>(m_edges);
        newSplitEdge->m_edgeIndexInVector = m_edges.size() - 1;
        std::get<2>(resTuple) = newSplitEdge;

        auto* newFace = createObject<typename ExtendedHalfEdgeTraits::FType>(m_faces);
        newFace->m_faceIndexInVector = m_faces.size() - 1;

        auto* newVertex = createObject<typename ExtendedHalfEdgeTraits::VType>(m_vertices);
        newVertex->m_vertexIndexInVector = m_vertices.size() - 1;

        //FIX THE FIRST FACE
        //fix the existing vertices
        nextHalfEdge->m_next->m_vertex->m_neighbourEdges.emplace_back(newSplitEdge);
        //fixed
        //fix the new vertices
        const glm::vec3 newVertexPos = (0.5f * (currEdge->m_firstVertex->m_position + currEdge->m_secondVertex->m_position));
        newVertex->m_position = newVertexPos;
        newVertex->m_halfEdge = currHalfEdge;
        newVertex->m_neighbourEdges.emplace_back(currEdge);
        newVertex->m_neighbourEdges.emplace_back(newSplitEdge);

        //fix the existing edges
        if (nextHalfEdge->m_vertex->m_position == currEdge->m_firstVertex->m_position) {
          currEdge->m_secondVertex = newVertex;
        }

        if (nextHalfEdge->m_vertex->m_position == currEdge->m_secondVertex->m_position) {
          currEdge->m_firstVertex = newVertex;
        }

        //fix the new edges
        newSplitEdge->m_halfEdge = newSplitHalfEdge;
        newSplitEdge->m_firstVertex = nextHalfEdge->m_next->m_vertex;
        newSplitEdge->m_secondVertex = newVertex;
        newSplitEdge->m_isOuter = false;

        //fix the existing faces
        ExtendedFace* currFace = currHalfEdge->m_face;
        currFace->m_halfEdge = currHalfEdge;

        //fix the new faces
        //fixed

        //fix the new halfEdges
        newSplitHalfEdge->m_face = currHalfEdge->m_face;
        newSplitHalfEdge->m_vertex = nextHalfEdge->m_next->m_vertex;
        newSplitHalfEdge->m_edge = newSplitEdge;
        newSplitHalfEdge->m_next = currHalfEdge;
        newSplitHalfEdge->m_previous = nextHalfEdge;
        newSplitHalfEdge->m_twin = newSplitHalfEdgeTwin;

        //fix the existing halfEdges
        //currHalfEdge->m_face = //done
        currHalfEdge->m_vertex = newVertex;
        //currHalfEdge->m_edge = //done
        //currHalfEdge->m_next = //done
        currHalfEdge->m_previous = newSplitHalfEdge;
        //currHalfEdge->m_twin = //done

        //nextHalfEdge->m_face = //done
        //nextHalfEdge->m_vertex = //done
        //nextHalfEdge->m_edge = //done
        nextHalfEdge->m_next = newSplitHalfEdge;
        //nextHalfEdge->m_previous = //done
        //nextHalfEdge->m_twin = //done

        //graphEdges
        for (int i = 0; i < currFace->m_graphEdges.size(); ++i) {
            if (currFace->m_graphEdges.at(i)->vertex == secondFaceVertex) {

                GraphEdge* graphEdge = currFace->m_graphEdges.at(i);

                int indexInFace = currFace->m_graphEdges.at(i)->graphEdgeIndexInFace;
                int indexInVertex = currFace->m_graphEdges.at(i)->graphEdgeIndexInVertex;

                secondFaceVertex->m_graphEdges.back()->graphEdgeIndexInVertex = indexInVertex;
                currFace->m_graphEdges.back()->graphEdgeIndexInFace = indexInFace;

                //vymazat
                utils::containers::swapLastAndPop(secondFaceVertex->m_graphEdges, indexInVertex);
                utils::containers::swapLastAndPop(currFace->m_graphEdges, indexInFace);

                //fix
                graphEdge->graphEdgeIndexInVertex = newVertex->m_graphEdges.size();
                graphEdge->vertex = newVertex;
                newVertex->m_graphEdges.emplace_back(graphEdge);

                graphEdge->graphEdgeIndexInFace = currFace->m_graphEdges.size();
                graphEdge->face = currFace;
                currFace->m_graphEdges.emplace_back(graphEdge);
            }
        }

        //sucastou druheho facu
        auto* newFirstHalfEdge = createObject<typename ExtendedHalfEdgeTraits::HType>(m_halfEdges);
        newFirstHalfEdge->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

        auto* newEdge = createObject<typename ExtendedHalfEdgeTraits::EType>(m_edges);
        newEdge->m_edgeIndexInVector = m_edges.size() - 1;
        std::get<1>(resTuple) = newEdge;

        //FIX THE SECOND FACE
        //fix the existing vertices
        if (secondFaceVertex->m_halfEdge == currHalfEdge) {
          secondFaceVertex->m_halfEdge = newFirstHalfEdge;
        }

        auto it = std::find(secondFaceVertex->m_neighbourEdges.begin(), secondFaceVertex->m_neighbourEdges.end(), edge);
        if (it != secondFaceVertex->m_neighbourEdges.end()) {
            secondFaceVertex->m_neighbourEdges.erase(it);
        }
        secondFaceVertex->m_neighbourEdges.emplace_back(newEdge);

        //fix the new vertices
        newVertex->m_neighbourEdges.emplace_back(newEdge);
        //fixed

        //fix the existing edges
        //fixed

        //fix the new edges
        newEdge->m_halfEdge = newFirstHalfEdge;
        newEdge->m_firstVertex = newVertex;
        newEdge->m_secondVertex = secondFaceVertex;
        newEdge->m_isOuter = true;

        //fix the existing faces
        //fixed

        //fix the new faces
        newFace->m_halfEdge = newFirstHalfEdge;

        //fix the existing halfEdges
        secondFaceHalfEdge->m_face = newFace;
        //secondFaceHalfEdge->m_vertex = //done
        //secondFaceHalfEdge->m_edge = //done
        secondFaceHalfEdge->m_next = newFirstHalfEdge;
        secondFaceHalfEdge->m_previous = newSplitHalfEdgeTwin;
        //secondFaceHalfEdge->m_twin = //done

        //fix the new halfEdges
        newFirstHalfEdge->m_face = newFace;
        newFirstHalfEdge->m_vertex = secondFaceVertex;
        newFirstHalfEdge->m_edge = newEdge;
        newFirstHalfEdge->m_next = newSplitHalfEdgeTwin;
        newFirstHalfEdge->m_previous = secondFaceHalfEdge;
        newFirstHalfEdge->m_twin = nullptr;

        newSplitHalfEdgeTwin->m_face = newFace;
        newSplitHalfEdgeTwin->m_vertex = newVertex;
        newSplitHalfEdgeTwin->m_edge = newSplitEdge;
        newSplitHalfEdgeTwin->m_next = secondFaceHalfEdge;
        newSplitHalfEdgeTwin->m_previous = newFirstHalfEdge;
        newSplitHalfEdgeTwin->m_twin = newSplitHalfEdge;

        //graphEdges
        ExtendedHalfEdge* halfEdge = newFace->m_halfEdge;
        do {
            //---ACCESS_EACH_VERTEX_OF_FACE---
            ExtendedVertex* vertex = halfEdge->m_vertex;
            halfEdge = halfEdge->m_next;

            //---CREATE_GRAPH_EDGE---
            GraphEdge* graphEdge = new GraphEdge();
            graphEdge->vertex = vertex;
            graphEdge->graphEdgeIndexInVertex = vertex->m_graphEdges.size();

            graphEdge->face = newFace;
            graphEdge->graphEdgeIndexInFace = newFace->m_graphEdges.size();

            vertex->m_graphEdges.emplace_back(graphEdge);
            newFace->m_graphEdges.emplace_back(graphEdge);

        } while (halfEdge != newFace->m_halfEdge);


        //twin face
        if (!currEdge->m_isOuter) {
            //sucastou prveho facu
            currHalfEdge = currHalfEdge->m_twin;
            ExtendedHalfEdge* previousHalfEdge = currHalfEdge->m_previous;

            secondFaceVertex = currHalfEdge->m_next->m_vertex;
            secondFaceHalfEdge = currHalfEdge->m_next;

            //nextHalfEdge = currHalfEdge->m_next;
            newSplitHalfEdge = createObject<typename ExtendedHalfEdgeTraits::HType>(m_halfEdges);
            newSplitHalfEdge->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

            newSplitHalfEdgeTwin = createObject<typename ExtendedHalfEdgeTraits::HType>(m_halfEdges);
            newSplitHalfEdgeTwin->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

            newSplitEdge = createObject<typename ExtendedHalfEdgeTraits::EType>(m_edges);
            newSplitEdge->m_edgeIndexInVector = m_edges.size() - 1;
            std::get<3>(resTuple) = newSplitEdge;

            newFace = createObject<typename ExtendedHalfEdgeTraits::FType>(m_faces);
            newFace->m_faceIndexInVector = m_faces.size() - 1;

            //FIX THE FIRST FACE
            //fix the existing vertices
            previousHalfEdge->m_vertex->m_neighbourEdges.emplace_back(newSplitEdge);
            //fixed
            //fix the new vertices
            newVertex->m_neighbourEdges.emplace_back(newSplitEdge);
            //fixed

            //fix the existing edges
            //fixed
            //fix the new edges
            newSplitEdge->m_halfEdge = newSplitHalfEdge;
            newSplitEdge->m_firstVertex = previousHalfEdge->m_vertex;
            newSplitEdge->m_secondVertex = newVertex;
            newSplitEdge->m_isOuter = false;

            //fix the existing faces
            currFace = currHalfEdge->m_face;
            currFace->m_halfEdge = currHalfEdge;

            //fix the new faces
            //fixed

            //fix the new halfEdges
            newSplitHalfEdge->m_face = currHalfEdge->m_face;
            newSplitHalfEdge->m_vertex = newVertex;
            newSplitHalfEdge->m_edge = newSplitEdge;
            newSplitHalfEdge->m_next = currHalfEdge->m_previous;
            newSplitHalfEdge->m_previous = currHalfEdge;
            newSplitHalfEdge->m_twin = newSplitHalfEdgeTwin;

            //fix the existing halfEdges
            //currHalfEdge->m_face = //done
            //currHalfEdge->m_vertex = //done
            //currHalfEdge->m_edge = //done
            currHalfEdge->m_next = newSplitHalfEdge;
            //currHalfEdge->m_previous = //done
            //currHalfEdge->m_twin = //done

            //previousHalfEdge->m_face = //done
            //previousHalfEdge->m_vertex = //done
            //previousHalfEdge->m_edge = //done
            //previousHalfEdge->m_next = //done
            previousHalfEdge->m_previous = newSplitHalfEdge;
            //previousHalfEdge->m_twin = //done

            //graphEdges
            for (int i = 0; i < currFace->m_graphEdges.size(); ++i) {
                if (currFace->m_graphEdges.at(i)->vertex == secondFaceVertex) {

                    GraphEdge* graphEdge = currFace->m_graphEdges.at(i);

                    int indexInFace = currFace->m_graphEdges.at(i)->graphEdgeIndexInFace;
                    int indexInVertex = currFace->m_graphEdges.at(i)->graphEdgeIndexInVertex;

                    secondFaceVertex->m_graphEdges.back()->graphEdgeIndexInVertex = indexInVertex;
                    currFace->m_graphEdges.back()->graphEdgeIndexInFace = indexInFace;

                    //vymazat
                    utils::containers::swapLastAndPop(secondFaceVertex->m_graphEdges, indexInVertex);
                    utils::containers::swapLastAndPop(currFace->m_graphEdges, indexInFace);

                    //fix
                    graphEdge->graphEdgeIndexInVertex = newVertex->m_graphEdges.size();
                    graphEdge->vertex = newVertex;
                    newVertex->m_graphEdges.emplace_back(graphEdge);

                    graphEdge->graphEdgeIndexInFace = currFace->m_graphEdges.size();
                    graphEdge->face = currFace;
                    currFace->m_graphEdges.emplace_back(graphEdge);
                }
            }


            //sucastou druheho facu
            auto* newSecondHalfEdge = createObject<ExtendedHalfEdgeTraits::HType>(m_halfEdges);
            newSecondHalfEdge->m_halfEdgeIndexInVector = m_halfEdges.size() - 1;

            //FIX THE SECOND FACE
            //fix the existing vertices
            //fixed
            //fix the new vertices
            //fixed

            //fix the existing edges
            //fixed

            //fix the new edges
            newEdge->m_isOuter = false;
            //fixed

            //fix the existing faces
            //fixed

            //fix the new faces
            newFace->m_halfEdge = newSecondHalfEdge;

            //fix the existing halfEdges
            secondFaceHalfEdge->m_face = newFace;
            //secondFaceHalfEdge->m_vertex = //done
            //secondFaceHalfEdge->m_edge = //done
            secondFaceHalfEdge->m_next = newSplitHalfEdgeTwin;
            secondFaceHalfEdge->m_previous = newSecondHalfEdge;
            //secondFaceHalfEdge->m_twin = //done

            //fix the new halfEdges
            newSecondHalfEdge->m_face = newFace;
            newSecondHalfEdge->m_vertex = newVertex;
            newSecondHalfEdge->m_edge = newEdge;
            newSecondHalfEdge->m_next = secondFaceHalfEdge;
            newSecondHalfEdge->m_previous = newSplitHalfEdgeTwin;
            newSecondHalfEdge->m_twin = newFirstHalfEdge;
            newFirstHalfEdge->m_twin = newSecondHalfEdge;

            newSplitHalfEdgeTwin->m_face = newFace;
            newSplitHalfEdgeTwin->m_vertex = previousHalfEdge->m_vertex;
            newSplitHalfEdgeTwin->m_edge = newSplitEdge;
            newSplitHalfEdgeTwin->m_next = newSecondHalfEdge;
            newSplitHalfEdgeTwin->m_previous = secondFaceHalfEdge;
            newSplitHalfEdgeTwin->m_twin = newSplitHalfEdge;

            //graphEdges
            halfEdge = newFace->m_halfEdge;
            do {
                //---ACCESS_EACH_VERTEX_OF_FACE---
                ExtendedVertex* vertex = halfEdge->m_vertex;
                halfEdge = halfEdge->m_next;

                //---CREATE_GRAPH_EDGE---
                GraphEdge* graphEdge = new GraphEdge();
                graphEdge->vertex = vertex;
                graphEdge->graphEdgeIndexInVertex = vertex->m_graphEdges.size();

                graphEdge->face = newFace;
                graphEdge->graphEdgeIndexInFace = newFace->m_graphEdges.size();

                vertex->m_graphEdges.emplace_back(graphEdge);
                newFace->m_graphEdges.emplace_back(graphEdge);

            } while (halfEdge != newFace->m_halfEdge);

        }

        return resTuple;
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


