#pragma once

#include "Callbacks/Callback.h"
#include "Patterns/Observer.h"
#include "Editing/Selection/SelectionController.h"
#include "Scene/Scene.h"
#include "Utils/GeometryUtils.h"
#include "ViewPortsController.h"
#include "DataStructures/Octree.h"
#include <glm/fwd.hpp>
#include "Callables/FacesVaoInitCallable.h"
#include "Callables/SceneFacesAdderCallable.h"
#include <chrono>

class SolidifyMeshesCallBack : public Callback<>, public Observer {
public:
	void execute() override{
        //najskor si musime ziskat SelectedMeshes
        SelectionController* selectionController = ViewPortsHolderContext::s_selectionController;
        Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
        SceneResources& sceneRes = scene->m_res;
        SceneResources::CoordsOctreeMap& coordsOctreeMap = sceneRes.coordsOctreeMap;
        const SelectionHolder& selectionHolder = selectionController->getHolder();
        const std::vector<Mesh*>& selectedMeshes = selectionHolder.meshes;
       
        //SHADOWING TEST
        /*
        for (Mesh* selectedMesh : selectedMeshes) {
            //musime ist cez vsetky selectedMeshes
            ExtendedHalfEdgeMesh* halfEdgeStructure = selectedMesh->getHalfEdgeStructure();
            //a musime teraz spravit to, ze budeme postupne prechadzat cez vsetky faces toho meshu

            for (ExtendedFace* selectedFace : halfEdgeStructure->m_faces) {
                
                std::vector<glm::vec3> selectedFace3DVertices;//zatial nepouzivame 
                std::vector<glm::vec2> selectedFace2DVertices; 
                
                ExtendedHalfEdge* currentHalfEdge = selectedFace->m_halfEdge;
                do {
                    ExtendedVertex* vertex = currentHalfEdge->m_vertex;
                    selectedFace3DVertices.emplace_back(vertex->m_position);
                    selectedFace2DVertices.emplace_back(glm::vec2(vertex->m_position.x, vertex->m_position.z));
                    currentHalfEdge = currentHalfEdge->m_next;

                } while (currentHalfEdge != selectedFace->m_halfEdge);



                //musime si vytvorit z toho velky aabb bounding volume
                //
                //AABBBoundingRegion(face->faceVertexBegin(), face->faceVertexEnd(), 
                glm::vec3 bounds[2];
                bounds[0] = glm::vec3(std::numeric_limits<float>::max());
                bounds[1] = glm::vec3(std::numeric_limits<float>::lowest());

                for(auto it = selectedFace->faceVertexBegin(); it != selectedFace->faceVertexEnd(); ++it){
                    glm::vec3 point = (*it).m_position;
                    
                    bounds[0].x = glm::min(bounds[0].x, point.x);
                    bounds[0].z = glm::min(bounds[0].z, point.z);
                    
                    bounds[1].x = glm::max(bounds[1].x, point.x);
                    bounds[1].z = glm::max(bounds[1].z, point.z);
                }

                AABBBoundingRegion aabb(bounds[0], bounds[1]);
                //mame uz ziskany ten infinite aabb bounding box
                //
                //teraz to musime ist skusit intersectnut so vsetkymi ostatnymi faces tohto daneho meshu
                //
                //
                //mame vytvoreny aabb bounding region, preto teraz musime ist cez vsetky octrees a skusit intersection test
                for(auto it = coordsOctreeMap.begin(); it != coordsOctreeMap.end(); ++it){
                    glm::vec3 coords = it->first;
                    Octree<SceneResources::MeshFacePair>& octree = it->second;
                    
                    const auto queryOctreeIntersectAlg = [](const AABBBoundingRegion& octreeBounds, const AABBBoundingRegion& queryBounds){
                        return octreeBounds.intersectsAABB(queryBounds);
                    };

                    std::vector<OctreeNode<SceneResources::MeshFacePair>*> returnedOctreeNodes;
                    octree.findMaxDepthNodes<AABBBoundingRegion>(aabb, queryOctreeIntersectAlg, returnedOctreeNodes);

                    for (OctreeNode<SceneResources::MeshFacePair>* octreeNode : returnedOctreeNodes) {
                        std::vector<SceneResources::MeshFacePair>& nodeData = octreeNode->nodeData;
                        
                        for (SceneResources::MeshFacePair& meshFacePair : nodeData) {
                            Mesh* mesh = meshFacePair.first;
                            ExtendedFace* face = meshFacePair.second;

                            if(mesh == selectedMesh && face != selectedFace){
                                //tak viem ze to moze byt
                                //get vertices as std::vector<glm::vec2>

                                std::vector<glm::vec3> face3DVertices;//zatial nepouzivame 
                                std::vector<glm::vec2> face2DVertices; 
                                
                                ExtendedHalfEdge* currentHalfEdge = face->m_halfEdge;
                                do {
                                    
                                    ExtendedVertex* vertex = currentHalfEdge->m_vertex;
                                    face3DVertices.emplace_back(vertex->m_position);
                                    face2DVertices.emplace_back(glm::vec2(vertex->m_position.x, vertex->m_position.z));
                                    currentHalfEdge = currentHalfEdge->m_next;

                                } while (currentHalfEdge != face->m_halfEdge);
                                //a teraz musime spravit to, ze skusime medzi face a selectedFace spravit intersectionTest
                                
                                //dobre mam uz face2Dvertices aj selectedFace2DVertices

                                if(utils::geometry::polygon2DOverlap(selectedFace2DVertices, face2DVertices)){
                                    ViewPortsHolderContext::s_selectionController->registerFace(mesh, face);
                                    std::cout << "SHADOWING HAPPENED" << std::endl;
                                    return;
                                }
                            }
                        }

                    }

                    //teraz musime ist cez vsetky tieto octreeNodes a 
                    
                    //dobre ziskal som vsetky octrees ktore sa s mojim jednym konkretnym aabb
                }

            }

        }
        std::cout << "SHADOWING NOT HAPPENING" << std::endl;
        */
        
        //musime ist teraz cez vsetky selected meshes
        //
        FaceParams faceParams;
        faceParams.faces = new std::vector<ExtendedFace*>();
        for (Mesh* selectedMesh : selectedMeshes) {
            faceParams.mesh = selectedMesh;
            
            ExtendedHalfEdgeMesh* halfEdgeStructure = selectedMesh->getHalfEdgeStructure();

            // Copy the current faces into a temporary vector
            std::cout << "Starting copying faces" << std::endl;
            auto start = std::chrono::high_resolution_clock::now();

            std::vector<ExtendedFace*> originalFaces = halfEdgeStructure->m_faces;

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;  // default = seconds
            std::cout << "Done copying faces in " << elapsed.count() << " s" << std::endl;

            for (ExtendedFace* selectedFace : originalFaces) {
                //musime ziskat projected vertices danej face a musime ich hlavne otocit
                
                //urobim si to manualne zatial
                ExtendedHalfEdge* currentHalfEdge = selectedFace->m_halfEdge;
                
                std::vector<glm::vec3> newFaceVertices;

                do{
                    ExtendedHalfEdge* previousHalfEdge = currentHalfEdge->m_previous;
                    ExtendedVertex* vertex = currentHalfEdge->m_vertex;
                    
                    glm::vec3 newVertexPosition = vertex->m_position;
                    newVertexPosition.y = -30.0f;
                    newFaceVertices.emplace_back(newVertexPosition);

                    currentHalfEdge = previousHalfEdge;
                } while(currentHalfEdge != selectedFace->m_halfEdge);

                ExtendedFace* newFace = halfEdgeStructure->addFloatingFace(newFaceVertices);

                faceParams.faces->emplace_back(newFace);
            }
        }

        FacesVaoInitCallable faceVaoInitCallable;
        faceVaoInitCallable.invoke(faceParams);

        SceneFacesAdderCallable sceneFaceAdderCallable;
        sceneFaceAdderCallable.invoke(faceParams);

        std::cout << "DONE SOLIDIFY" << std::endl;

    }
}; 
