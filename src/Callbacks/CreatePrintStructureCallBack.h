#pragma once

#include "Callback.h"
#include "Structures/PrintableMesh.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Utils/GeometryUtils.h"
#include "Utils/glm_overrides.h"
#include "ViewPortsController.h"
#include "Params/OperationParams.h"
#include "UI/OutlinerLayer.h"
#include <print>
#include "../Renderer/MaterialRegistry.h"

class CreatePrintStructureCallBack : public Callback<PrintMeshSettingsParams>, public Observer {

public:
	void execute(const PrintMeshSettingsParams& iParams) override {

        //Get or create Printable Mesh From SceneResources
        float perimeterHeight = iParams.height;
        Mesh* inputMesh = iParams.mesh;
        SceneResources& sceneResources = ViewPortsHolderContext::s_viewPortsController->m_scene->m_res;
        
        auto [printableMeshIt, _] = sceneResources.printableMeshMap.try_emplace(inputMesh, new PrintableMesh());
        PrintableMesh* outputPrintableMesh = printableMeshIt->second;

        ViewPortsController* controller = ViewPortsHolderContext::s_viewPortsController;
        Scene* scene = controller->m_scene;
        SceneResources::CoordsOctreeMap& octreeCoordsMap = scene->m_res.coordsOctreeMap;
        
        //Go through all plane slices of the mesh
        float minPlaneY = inputMesh->m_meshBounds.getMinBoundsPos().y;
        float maxPlaneY = inputMesh->m_meshBounds.getMaxBoundsPos().y;
        minPlaneY = (std::trunc(minPlaneY * 10.0f) / 10.0f) + 0.1f;

        Plane plane;
        plane.point = glm::vec3(0.0f, minPlaneY, 0.0f);
        plane.normal = glm::vec3(0.0f, 1.0f, 0.0f);

        for (float planeHeight = plane.point.y; plane.point.y < maxPlaneY; plane.point.y += perimeterHeight) {
            //vytvor plane
            std::map<std::pair<glm::vec3, glm::vec3>, std::pair<ExtrudeEdge, ExtendedFace*>, glm::Vec3PairApproxComparator> printableEdgesMap;

            //Go through all octrees in the scene
            for(auto& octreeCoordsPair : octreeCoordsMap) {
                auto& [coords, octree] = octreeCoordsPair;

                auto octreeBoundsIntersectAlg = [](const AABBBoundingRegion& aabb, const Plane& plane){
                    return aabb.insersectsPlane(plane);
                };

                //Get all leaf nodes that intersected with the plane
                std::vector<OctreeNode<SceneResources::MeshFacePair>*> hitOctreeNodes;
                octree.findMaxDepthNodes<Plane>(plane, octreeBoundsIntersectAlg, hitOctreeNodes);
                
                //Go through each leaf node
                for(OctreeNode<SceneResources::MeshFacePair>* octreeNode : hitOctreeNodes) {
                    std::vector<SceneResources::MeshFacePair>& nodeData = octreeNode->nodeData;
                    
                    //Go through each face
                    for(SceneResources::MeshFacePair& data : nodeData) {
                        auto& [mesh, face] = data;
                        
                        if(inputMesh == mesh) {
                            //Try plane-face intersection 
                            utils::geometry::PlaneFaceIntersection planeIntersection = utils::geometry::planeIntersectsFace(plane, face);
                            
                            //If line intersected at two points of a triangle, then add that to the printable edges
                            if(planeIntersection.type == utils::geometry::PlaneFaceIntersection::Type::Edge){

                                glm::vec3 modifiedFirstPoint {planeIntersection.firstPoint.x, plane.point.y, planeIntersection.firstPoint.z};
                                glm::vec3 modifiedSecondPoint {planeIntersection.secondPoint.x, plane.point.y, planeIntersection.secondPoint.z};

                                printableEdgesMap.insert({
                                    {modifiedFirstPoint, modifiedSecondPoint},
                                    {ExtrudeEdge{modifiedFirstPoint, modifiedSecondPoint}, face}
                                });
                            }
                        }
                    }
                }
            }
            
            //Convert PrintableEdgesMap into a std::vector type
            std::vector<std::pair<ExtrudeEdge, ExtendedFace*>> printableEdges;
            printableEdges.reserve(printableEdgesMap.size());

            for (auto& [_, value] : printableEdgesMap) {
                printableEdges.emplace_back(value);
            }

            outputPrintableMesh->addPerimeterLayerLevel(inputMesh, printableEdges);
        }

        //Register Printable Mesh BufferStorage
        Material* defaultLineMaterial = MaterialRegistry::getMaterial("defaultLineMaterial");
        LineBufferStorage& lineBufferStorage = outputPrintableMesh->bufferLayout.registerLineBufferStorage(defaultLineMaterial);
        lineBufferStorage.create();

        //Get Raw Mesh Buffer Data For Specified Mesh
        std::vector<BufferStorageDataType::LineVertex>& lineBufferVertices = lineBufferStorage.data.vertices;
        lineBufferVertices.clear();


        //Update PrintableMesh info
        for(PrintLayerLevel& printLayerLevel : outputPrintableMesh->m_structure.printLayers) {
            std::vector<PerimeterOutline>& perimeterOutlines = printLayerLevel.perimeterOutlines;
            for(PerimeterOutline& perimeterOutline : perimeterOutlines) {

                for(auto it = perimeterOutline.points.begin(); it != perimeterOutline.points.end(); ++it) {
                     
                    if(perimeterOutline.filled) {
                        lineBufferVertices.emplace_back(it->first.firstPoint, true);
                        lineBufferVertices.emplace_back(it->first.secondPoint, true);
                    } else {
                        lineBufferVertices.emplace_back(it->first.firstPoint, 0.25f);
                        lineBufferVertices.emplace_back(it->first.secondPoint, 0.25f);
                    }
                }
            }
            
            std::vector<InfillLine>& infillLines = printLayerLevel.infillLines;

            for(InfillLine& infillLine : infillLines) {
                for(auto it = infillLine.points.begin(); it != infillLine.points.end(); std::advance(it,2)) {

                    lineBufferVertices.emplace_back(it->pos, 0.5f);
                    lineBufferVertices.emplace_back(std::next(it)->pos, 0.5f);
                }
            }

        }

        lineBufferStorage.update();

        //Add Printable Mesh into the scene
        SceneResources::PrintableMeshMap& printableMeshesMap = scene->m_res.printableMeshMap;
        auto [it, inserted] = printableMeshesMap.emplace(inputMesh, outputPrintableMesh); 
        
        //Add created printable mesh into the outliner layer
        if(OutlinerLayer::m_state.m_currentSelectedNode != nullptr) {
            OutlinerLayer::addChildNode(OutlinerLayer::m_state.m_currentSelectedNode, 1, "PrintableMesh1", outputPrintableMesh);
        }

    }
};
