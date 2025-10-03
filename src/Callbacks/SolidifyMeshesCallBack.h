#pragma once

#include "Callbacks/Callback.h"
#include "Callbacks/ConnectEdgesCallBack.h"
#include "Params/OperationParams.h"
#include "Patterns/Observer.h"
#include "Editing/Selection/SelectionController.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Utils/GeometryUtils.h"
#include "Utils/glm_overrides.h"
#include "ViewPortsController.h"
#include "Structures/Octree.h"
#include <glm/fwd.hpp>
#include "Callables/FacesVaoInitCallable.h"
#include "Callables/SceneFacesAdderCallable.h"
#include "Callables/EdgesVaoInitCallable.h"
#include "Callables/FaceVaoInitCallable.h"
#include <chrono>

class SolidifyMeshesCallBack : public Callback<>, public Observer {

public:

	void execute() override {
        //Get Selected Meshes 
        SelectionController* selectionController = ViewPortsHolderContext::s_selectionController;
        Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
        SceneResources& sceneRes = scene->m_res;
        SceneResources::CoordsOctreeMap& coordsOctreeMap = sceneRes.coordsOctreeMap;
        const SelectionHolder& selectionHolder = selectionController->getHolder();
        const std::vector<Mesh*>& selectedMeshes = selectionHolder.meshes;
      

        for (Mesh* selectedMesh : selectedMeshes) {
            
            ExtendedHalfEdgeMesh* halfEdgeStructure = selectedMesh->getHalfEdgeStructure();

            std::vector<ExtendedFace*> originalFaces = halfEdgeStructure->m_faces;
            
            std::map<std::pair<glm::vec3, glm::vec3>, ExtendedEdge*, glm::Vec3PairApproxComparator> usedEdgesMap;
            std::map<std::pair<glm::vec3, glm::vec3>, std::pair<ExtendedEdge*, ExtendedEdge*>, glm::Vec3PairApproxComparator> outerEdgesMap;
            std::vector<ExtendedFace*> toBeAddedOctreeFaces;


            for (ExtendedFace* selectedFace : originalFaces) {

                ExtendedHalfEdge* currentHalfEdge = selectedFace->m_halfEdge;
                
                std::vector<glm::vec3> newFaceVertices;

                do{
                    ExtendedEdge* currentEdge = currentHalfEdge->m_edge;

                    ExtendedHalfEdge* previousHalfEdge = currentHalfEdge->m_previous;
                    
                    glm::vec3 newVertexPosition = currentHalfEdge->m_vertex->m_position; 
                    newVertexPosition.y = -60.0f;
                    newFaceVertices.emplace_back(newVertexPosition);
                    
                    glm::vec3 newEdgeFirstVertexPos = currentEdge->m_firstVertex->m_position;
                    glm::vec3 newEdgeSecondVertexPos = currentEdge->m_secondVertex->m_position;
                    
                    newEdgeFirstVertexPos.y = -60.0f;
                    newEdgeSecondVertexPos.y = -60.0f;

                    if(currentEdge->m_isOuter){
                        usedEdgesMap.insert({std::make_pair(currentEdge->m_firstVertex->m_position, currentEdge->m_secondVertex->m_position), currentEdge});

                        outerEdgesMap.insert({std::make_pair(newEdgeFirstVertexPos, newEdgeSecondVertexPos), 
                                std::make_pair(currentEdge, nullptr)});
                    }

                    currentHalfEdge = previousHalfEdge;
                } while(currentHalfEdge != selectedFace->m_halfEdge);
                
                ExtendedFace* newFace = halfEdgeStructure->addFloatingFace(newFaceVertices);
                toBeAddedOctreeFaces.emplace_back(newFace);

                FaceVaoInitCallable faceVaoInitCallable;
                NewFaceParams newFaceParams;
                newFaceParams.face = newFace;
                newFaceParams.mesh = selectedMesh;
                faceVaoInitCallable.invoke(newFaceParams);

                std::vector<ExtendedEdge*> edges;
                for (auto it = newFace->faceHalfEdgeEnd(); it != newFace->faceHalfEdgeEnd(); ++it) {
                    ExtendedEdge* edge = (*it).m_edge;
                    edges.emplace_back(edge);
                }
                EdgesVaoInitCallable edgesVaoInitCallable;
                EdgeParams edgeParams;
                edgeParams.edges = &edges;
                edgeParams.mesh = selectedMesh;
                edgesVaoInitCallable.invoke(edgeParams);

                
                for(auto it = newFace->faceHalfEdgeBegin(); it != newFace->faceHalfEdgeEnd(); ++it) {
                    ExtendedEdge* edge = (*it).m_edge;

                    auto outerEdgesMapIt = outerEdgesMap.find(std::make_pair(edge->m_firstVertex->m_position, edge->m_secondVertex->m_position));

                    if(outerEdgesMapIt != outerEdgesMap.end()) {
                        std::pair<ExtendedEdge*, ExtendedEdge*>& outerEdgesPair = outerEdgesMapIt->second;
                        outerEdgesPair.second = edge;
                    }
                   

                    auto [usedEdgesMapIt, inserted] = usedEdgesMap.try_emplace(std::make_pair(edge->m_firstVertex->m_position, edge->m_secondVertex->m_position), edge);
                    if(!inserted){
                        ExtendedEdge* otherEdge = usedEdgesMapIt->second;

                        ConnectEdgesCallBack connectEdgesCallBack;
                        EdgeConnectionsParams edgeConnectionsParams;
                        edgeConnectionsParams.mesh = selectedMesh;
                        edgeConnectionsParams.firstEdge = edge;
                        edgeConnectionsParams.secondEdge = otherEdge;
                        connectEdgesCallBack.execute(edgeConnectionsParams);

                        usedEdgesMap.erase(usedEdgesMapIt);
                    }
                }
                
            }


            for (auto& [_, edgesPair] : outerEdgesMap) {
                ExtendedHalfEdge* upperHalfEdge = edgesPair.first->m_halfEdge;
                ExtendedHalfEdge* bottomHalfEdge = edgesPair.second->m_halfEdge;

                 
                std::vector<glm::vec3> firstOuterFaceVerts {
                    bottomHalfEdge->m_vertex->m_position,
                    upperHalfEdge->m_next->m_vertex->m_position,
                    bottomHalfEdge->m_next->m_vertex->m_position
                };

                std::vector<glm::vec3> secondOuterFaceVerts {
                    bottomHalfEdge->m_next->m_vertex->m_position,
                    upperHalfEdge->m_next->m_vertex->m_position,
                    upperHalfEdge->m_vertex->m_position
                };

                ExtendedFace* firstOuterFace = halfEdgeStructure->addFloatingFace(firstOuterFaceVerts);
                toBeAddedOctreeFaces.emplace_back(firstOuterFace);

                FaceVaoInitCallable faceVaoInitCallable1;
                NewFaceParams newFaceParams1;
                newFaceParams1.face = firstOuterFace;
                newFaceParams1.mesh = selectedMesh;
                faceVaoInitCallable1.invoke(newFaceParams1);

                std::vector<ExtendedEdge*> edges1;
                for (auto it = firstOuterFace->faceHalfEdgeEnd(); it != firstOuterFace->faceHalfEdgeEnd(); ++it) {
                    ExtendedEdge* edge = (*it).m_edge;
                    edges1.emplace_back(edge);
                }
                EdgesVaoInitCallable edgesVaoInitCallable1;
                EdgeParams edgeParams1;
                edgeParams1.edges = &edges1;
                edgeParams1.mesh = selectedMesh;
                edgesVaoInitCallable1.invoke(edgeParams1);


                for(auto it = firstOuterFace->faceHalfEdgeBegin(); it != firstOuterFace->faceHalfEdgeEnd(); ++it) {
                    ExtendedEdge* edge = (*it).m_edge;

                    auto [usedEdgesMapIt, inserted] = usedEdgesMap.try_emplace(std::make_pair(edge->m_firstVertex->m_position, edge->m_secondVertex->m_position), edge);
                    if(!inserted){
                        ExtendedEdge* otherEdge = usedEdgesMapIt->second;

                        ConnectEdgesCallBack connectEdgesCallBack;
                        EdgeConnectionsParams edgeConnectionsParams;
                        edgeConnectionsParams.mesh = selectedMesh;
                        edgeConnectionsParams.firstEdge = edge;
                        edgeConnectionsParams.secondEdge = otherEdge;
                        connectEdgesCallBack.execute(edgeConnectionsParams);

                        usedEdgesMap.erase(usedEdgesMapIt);
                    }

                }

                ExtendedFace* secondOuterFace = halfEdgeStructure->addFloatingFace(secondOuterFaceVerts);
                toBeAddedOctreeFaces.emplace_back(secondOuterFace);

                FaceVaoInitCallable faceVaoInitCallable2;
                NewFaceParams newFaceParams2;
                newFaceParams2.face = secondOuterFace;
                newFaceParams2.mesh = selectedMesh;
                faceVaoInitCallable2.invoke(newFaceParams2);

                std::vector<ExtendedEdge*> edges2;
                for (auto it = secondOuterFace->faceHalfEdgeEnd(); it != secondOuterFace->faceHalfEdgeEnd(); ++it) {
                    ExtendedEdge* edge = (*it).m_edge;
                    edges2.emplace_back(edge);
                }
                EdgesVaoInitCallable edgesVaoInitCallable2;
                EdgeParams edgeParams2;
                edgeParams2.edges = &edges2;
                edgeParams2.mesh = selectedMesh;
                edgesVaoInitCallable2.invoke(edgeParams2);


                for(auto it = secondOuterFace->faceHalfEdgeBegin(); it != secondOuterFace->faceHalfEdgeEnd(); ++it) {
                    ExtendedEdge* edge = (*it).m_edge;

                    
                    auto [usedEdgesMapIt, inserted] = usedEdgesMap.try_emplace(std::make_pair(edge->m_firstVertex->m_position, edge->m_secondVertex->m_position), edge);
                    if(!inserted){
  
                        ExtendedEdge* otherEdge = usedEdgesMapIt->second;

                        ConnectEdgesCallBack connectEdgesCallBack;
                        EdgeConnectionsParams edgeConnectionsParams;
                        edgeConnectionsParams.mesh = selectedMesh;
                        edgeConnectionsParams.firstEdge = edge;
                        edgeConnectionsParams.secondEdge = otherEdge;
                        connectEdgesCallBack.execute(edgeConnectionsParams);

                        usedEdgesMap.erase(usedEdgesMapIt);
                    }

                }
            }


            LineBufferStorage* lineBufferStorage = Renderer::s_bufferRegistry.queryBuffer<LineBufferStorage>();
            lineBufferStorage->updateBufferStorage(selectedMesh);

            MeshBufferStorage* meshBufferStorage = Renderer::s_bufferRegistry.queryBuffer<MeshBufferStorage>();
            meshBufferStorage->updateBufferStorage(selectedMesh, selectedMesh->m_defaultMaterial);

            SceneFacesAdderCallable sceneFacesAdderCallable;
            FaceParams faceParams;
            faceParams.mesh = selectedMesh;
            faceParams.faces = &toBeAddedOctreeFaces;
            sceneFacesAdderCallable.invoke(faceParams);

            selectedMesh->calculateMeshBounds();
        }
    }
}; 
