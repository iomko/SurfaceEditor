#pragma once

#include "Callback.h"
#include "DataStructures/PrintableMesh.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Utils/GeometryUtils.h"
#include "ViewPortsController.h"
#include "Params/OperationParams.h"
#include "UI/OutlinerLayer.h"
#include <print>

class CreatePrintStructureCallBack : public Callback<PrintMeshSettingsParams>, public Observer {

private:
struct Vec3LessEps {
    static constexpr float eps = 1e-6f;

    bool operator()(const glm::vec3& a, const glm::vec3& b) const {
        if (std::abs(a.x - b.x) > eps) return a.x < b.x;
        if (std::abs(a.y - b.y) > eps) return a.y < b.y;
        if (std::abs(a.z - b.z) > eps) return a.z < b.z;
        return false; // equal within epsilon
    }
};

struct EdgeComparatorEps {
    bool operator()(const std::pair<glm::vec3, glm::vec3>& lhs,
                    const std::pair<glm::vec3, glm::vec3>& rhs) const {
        auto l = std::minmax(lhs.first, lhs.second, Vec3LessEps{});
        auto r = std::minmax(rhs.first, rhs.second, Vec3LessEps{});

        if (Vec3LessEps{}(l.first, r.first)) return true;
        if (Vec3LessEps{}(r.first, l.first)) return false;
        return Vec3LessEps{}(l.second, r.second);
    }
};

public:
	void execute(const PrintMeshSettingsParams& iParams) override {
        float perimeterHeight = iParams.height;
        Mesh* inputMesh = iParams.mesh;
        SceneResources& sceneResources = ViewPortsHolderContext::s_viewPortsController->m_scene->m_res;
        
        auto [printableMeshIt, _] = sceneResources.printableMeshMap.try_emplace(inputMesh, new PrintableMesh());
        PrintableMesh* outputPrintableMesh = printableMeshIt->second;
        //takze najskor musime spravit to ze si ziskame Mesh, na ktory aplikujeme tuto metodu

        //nemozeme zakazdym vytvarat novy printableMesh samozrejme

        ViewPortsController* controller = ViewPortsHolderContext::s_viewPortsController;
        Scene* scene = controller->m_scene;
        SceneResources::CoordsOctreeMap& octreeCoordsMap = scene->m_res.coordsOctreeMap;
        
        //budeme prechadzat cez vsetky planes, zo zaciatku len pre test ich spravime napr 10
        Plane plane{{0.0f, -50.0f, 0.0f}, {0.0f, 0.1f, 0.0f} };
        for (int planeIndex = 0; planeIndex < 200; planeIndex++) {
            //vytvor plane
            plane.point.y += perimeterHeight;
            std::map<std::pair<glm::vec3, glm::vec3>, ExtrudeEdge, EdgeComparatorEps> extrudeEdgesMap;


            for(auto& octreeCoordsPair : octreeCoordsMap) {
                auto& [coords, octree] = octreeCoordsPair;


                auto octreeBoundsIntersectAlg = [](const AABBBoundingRegion& aabb, const Plane& plane){
                    return aabb.insersectsPlane(plane);
                };
                std::vector<OctreeNode<SceneResources::MeshFacePair>*> hitOctreeNodes;
                octree.findMaxDepthNodes<Plane>(plane, octreeBoundsIntersectAlg, hitOctreeNodes);

                for(OctreeNode<SceneResources::MeshFacePair>* octreeNode : hitOctreeNodes) {
                    std::println("went inside of octreeNodes in printing structure");
                    std::vector<SceneResources::MeshFacePair>& nodeData = octreeNode->nodeData;

                    for(SceneResources::MeshFacePair& data : nodeData) {
                        auto& [mesh, face] = data;
                        
                        //dobre a teraz sa musime pozriet ze ci vobec tato face patri danemu meshu,
                        //lebo ak nie, tak ju nebereme do uvahy
                        if(inputMesh == mesh) {
                            //tak mozeme ist dalej
                            //a teraz co vlastne musim spravit je to, ze skusim intersection between plane a touto face(triangle)
                            utils::geometry::PlaneFaceIntersection planeIntersection = utils::geometry::planeIntersectsFace(plane, face);
                            if(planeIntersection.type == utils::geometry::PlaneFaceIntersection::Type::Edge){
                                //tak vieme ze uz mame edge
                                
                                extrudeEdgesMap.insert({
                                    {planeIntersection.firstPoint, planeIntersection.secondPoint},
                                    ExtrudeEdge{planeIntersection.firstPoint, planeIntersection.secondPoint}
                                });
                                //extrudeEdges.emplace_back(planeIntersection.firstPoint, planeIntersection.secondPoint);
                            }
                        }
                    }
                }
            }
            
            //mame uz vsetky extrude edges pre dany plane
            std::vector<ExtrudeEdge> extrudeEdges;
            extrudeEdges.reserve(extrudeEdgesMap.size()); // avoid reallocations

            for (auto& [_, value] : extrudeEdgesMap) {
                extrudeEdges.push_back(value);
            }

            outputPrintableMesh->addLevelLayers(extrudeEdges);
        }

        //super teraz uz mame vytvorene samotne layers, teraz co potrebujeme je moznost to pridat do sceny nech to clovek vidi
        //to znamena ze teraz potrebujeme separatny buffer na to aby som mohol vykreslit zase tuto PrintableMesh strukturu
        PrintableMeshBufferStorage* printableMeshBufferStorage = Renderer::s_bufferRegistry.queryBuffer<PrintableMeshBufferStorage>();
        printableMeshBufferStorage->registerBufferStorage(outputPrintableMesh);

        BufferData<RendererBuffersData::LineVertex>* printableMeshBufferData;
        printableMeshBufferStorage->getBufferData(outputPrintableMesh, printableMeshBufferData);
        std::vector<RendererBuffersData::LineVertex>& edgesVector = printableMeshBufferData->vertices;
        edgesVector.clear();

        for(PrintLayer& printLayer : outputPrintableMesh->m_perimeterLayer) {
            std::vector<std::list<ExtrudePoint>>& levelLayers = printLayer.levelLayers;
            
            for(std::list<ExtrudePoint>& levelLayer : levelLayers) {

                for(auto it = levelLayer.begin(); it != levelLayer.end(); ++it) {
                    auto nextIt = std::next(it);
                    if(nextIt == levelLayer.end()) {
                        nextIt = levelLayer.begin();
                    }
                    edgesVector.emplace_back(it->pos, true);
                    edgesVector.emplace_back(nextIt->pos, true);
                }

            }

        }

        //pridaj do sceny
        //
        SceneResources::PrintableMeshMap& printableMeshesMap = scene->m_res.printableMeshMap;

        auto [it, inserted] = printableMeshesMap.emplace(inputMesh, outputPrintableMesh); 
    
        printableMeshBufferStorage->updateBufferStorage(outputPrintableMesh);
        
        //pridaj do outlinerLayer


        //OutlinerLayer::addNode(0, "Mesh", mesh);
        if(OutlinerLayer::m_state.m_currentSelectedNode != nullptr) {
            OutlinerLayer::addChildNode(OutlinerLayer::m_state.m_currentSelectedNode, 1, "PrintableMesh1", outputPrintableMesh);
        }
        //

        std::println("Done creating print structure!");
        //

    }
};
