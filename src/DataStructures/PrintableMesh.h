#pragma once
#include <limits>
#include <vector>
#include <list>
#include <glm/glm.hpp>
#include <variant>
#include <map>
#include "DataStructures/ExtendedHalfEdge.h"
#include "DataStructures/QuadTree.h"
#include "Primitives/BoundingRegion2D.h"
#include "Primitives/Plane.h"
#include "Utils/ContainerUtils.h"
#include "Utils/GeometryUtils.h"
#include "Ray2D.h"
#include "Scene/Mesh.h"


struct ExtrudeEdge {
    ExtrudeEdge(const glm::vec3& firstPoint, const glm::vec3& secondPoint) {
        this->firstPoint = firstPoint;
        this->secondPoint = secondPoint;
    }
    glm::vec3 firstPoint;
    glm::vec3 secondPoint;

    bool operator<(const ExtrudeEdge& other) const {
        if (firstPoint.x != other.firstPoint.x) return firstPoint.x < other.firstPoint.x;
        if (firstPoint.y != other.firstPoint.y) return firstPoint.y < other.firstPoint.y;
        if (firstPoint.z != other.firstPoint.z) return firstPoint.z < other.firstPoint.z;

        if (secondPoint.x != other.secondPoint.x) return secondPoint.x < other.secondPoint.x;
        if (secondPoint.y != other.secondPoint.y) return secondPoint.y < other.secondPoint.y;
        return secondPoint.z < other.secondPoint.z;
    }

    void swapPoints() {
        std::swap(firstPoint, secondPoint);
    }
};

struct OutlinerEdgeHelperData {
    OutlinerEdgeHelperData(const glm::vec3& firstPoint, const glm::vec3 secondPoint, bool filled, int outlineId) 
        : m_edge(firstPoint, secondPoint), m_filled(filled), m_outlineId(outlineId) {}

    ExtrudeEdge m_edge;
    bool m_filled;
    int m_outlineId;
    glm::vec2 m_intersectionPoint{};

    bool operator<(const OutlinerEdgeHelperData& other) const {
        return m_edge < other.m_edge;
    }
};


struct Vec2Comparator {
    bool operator()(const OutlinerEdgeHelperData& a, const OutlinerEdgeHelperData& b) const {
        if (a.m_intersectionPoint.x != b.m_intersectionPoint.x) return a.m_intersectionPoint.x < b.m_intersectionPoint.x;
        return a.m_intersectionPoint.y < b.m_intersectionPoint.y;
    }
};

struct ExtrudePoint {
    glm::vec3 pos;
};

struct PerimeterOutline {
    std::list<std::pair<ExtrudeEdge, ExtendedFace*>> points;
    utils::geometry::WindingOrder windingOrder = utils::geometry::WindingOrder::None;
    bool filled = false;
};

struct InfillLine {
    std::list<ExtrudePoint> points;
};

struct PrintLayerLevel {
    std::vector<PerimeterOutline> perimeterOutlines;
    std::vector<InfillLine> infillLines;
};

struct PrintableMeshStructure {
    std::vector<PrintLayerLevel> printLayers;
};

class PrintableMesh {
private:
    struct FrontTag {};
    struct BackTag {};
public:
    PrintableMesh() {
    
    }

    void removeAllLevelLayers() {
        m_structure.printLayers.clear();
    }

    void test() {

    }

    void updateQuadtreeBounds(const ExtrudeEdge& edge, glm::vec2& minBounds, glm::vec2& maxBounds) {
        minBounds.x = std::min(minBounds.x, std::min(edge.firstPoint.x, edge.secondPoint.x));
        minBounds.y = std::min(minBounds.y, std::min(edge.firstPoint.z, edge.secondPoint.z));

        maxBounds.x = std::max(maxBounds.x, std::max(edge.firstPoint.x, edge.secondPoint.x));
        maxBounds.y = std::max(maxBounds.y, std::max(edge.firstPoint.z, edge.secondPoint.z));
    }

    /*
    void updateQuadtreeBounds(const ExtrudeEdge& edge, glm::vec2& minBounds, glm::vec2& maxBounds) {
        //update min
        minBounds.x = std::min({edge.firstPoint.x, edge.secondPoint.x, minBounds.x});
        minBounds.y = std::min({edge.firstPoint.y, edge.secondPoint.y, minBounds.y});

        //update max
        maxBounds.x = std::max({edge.firstPoint.x, edge.secondPoint.x, maxBounds.x});
        maxBounds.y = std::max({edge.firstPoint.y, edge.secondPoint.y, maxBounds.y});
    }
    */

    void addInfillLayerLevel(Quadtree<OutlinerEdgeHelperData>& outlinerQuadtree, float yTemp) {
        std::vector<InfillLine>& infillLines = m_structure.printLayers.back().infillLines;

        BoundingRegion2D& quadTreeBounds = outlinerQuadtree.rootNode->nodeBounds;
        glm::vec2 minPoint = quadTreeBounds.getMinBoundsPos();
        glm::vec2 maxPoint = quadTreeBounds.getMaxBoundsPos();

        float xLength = maxPoint.x - minPoint.y;
        float yLength = maxPoint.y - minPoint.y;
        float maxSideLength = std::max(xLength, yLength);

        glm::vec2 modifiedMinPoint = minPoint;
        glm::vec2 modifiedMaxPoint{minPoint.x + maxSideLength, minPoint.y + maxSideLength};

        float offsetLength = glm::distance(modifiedMinPoint, modifiedMaxPoint);
        glm::vec2 offsetDir = glm::normalize(modifiedMaxPoint - modifiedMinPoint);
        glm::vec2 oppositeOffsetDir(-offsetDir.y, offsetDir.x);

        //teraz musime vytvorit dva krajne body na ciare
        float incrementNum = 5.0f;
        glm::vec2 currentOffsetPosition = modifiedMinPoint + (offsetDir * incrementNum);
        while (currentOffsetPosition.x < modifiedMaxPoint.x && currentOffsetPosition.y < modifiedMaxPoint.y) {
            
            Ray2D ray;
            ray.origin = currentOffsetPosition + (oppositeOffsetDir * offsetLength);
            ray.direction = -oppositeOffsetDir;

            currentOffsetPosition = currentOffsetPosition + (offsetDir * incrementNum);

            //teraz uz mame aj ray, tak mozeme spravit ray intersect s quadTree,
            //cim si ziskame jednotlive leaf nodes, ktore sa s tymto ray intersectovali
            auto rayIntersectsAlg = [](const BoundingRegion2D& boundingRegion, const Ray2D& ray){
                return boundingRegion.intersectsRay(ray); 
            };
            std::vector<QuadtreeNode<OutlinerEdgeHelperData>*> returnedQuadtreeNodes;
            outlinerQuadtree.findMaxDepthNodes<Ray2D>(ray, rayIntersectsAlg, returnedQuadtreeNodes);

            //musime sa pozriet ze ci nahodou uz neexistuju rovnake data v inych leafs
            //we only want unique edges
            std::set<ExtrudeEdge> uniqueEdges;
            std::set<OutlinerEdgeHelperData, Vec2Comparator> orderedPoints; 

            for(QuadtreeNode<OutlinerEdgeHelperData>* quadtreeLeaf : returnedQuadtreeNodes) {
                for(OutlinerEdgeHelperData& data : quadtreeLeaf->nodeData) {

                    glm::vec2 intersectionPoint;
                   
                    /*
                    if(ray.intersectSegment({data.m_edge.firstPoint, data.m_edge.secondPoint}, intersectionPoint)){
                        std::cout << "intersect" << std::endl;
                    } else {
                        ray.direction = -ray.direction; 
                        if(ray.intersectSegment({data.m_edge.firstPoint, data.m_edge.secondPoint}, intersectionPoint)){
                            std::cout << "intersect diff" << std::endl;
                        }
                    }
                    */
                    Segment2D segment{{data.m_edge.firstPoint.x, data.m_edge.firstPoint.z},{data.m_edge.secondPoint.x, data.m_edge.secondPoint.z}};

                    if(ray.intersectSegment(segment, intersectionPoint)){
                        auto [it, inserted] = uniqueEdges.insert(data.m_edge);
                        if(inserted){
                            data.m_intersectionPoint = intersectionPoint;
                            orderedPoints.insert(data);
                        }
                    }
                }
            }

            //uz mame k dispozicii aj orderedPoints
            //teraz co este musim spravit je to, ze urobim tu logiku stacku
            //
            bool firstInfillLine = true;
            std::vector<OutlinerEdgeHelperData> stack;
            for(const OutlinerEdgeHelperData& data : orderedPoints) {
                if(!stack.empty()){
                    //pridame ciaru alebo nie
                    //
                    if(stack.back().m_filled) {
                        if(firstInfillLine){
                            infillLines.emplace_back();
                            firstInfillLine = false;
                        }
                        //tak pridaj
                        infillLines.back().points.emplace_back(glm::vec3(stack.back().m_intersectionPoint.x, yTemp, stack.back().m_intersectionPoint.y));
                        infillLines.back().points.emplace_back(glm::vec3(data.m_intersectionPoint.x, yTemp, data.m_intersectionPoint.y));
                    }

                    if(stack.back().m_outlineId == data.m_outlineId) {
                        //tak odstran posledne data zo stacku
                        stack.pop_back();
                        if(!stack.empty()){
                            stack.back().m_intersectionPoint = data.m_intersectionPoint;
                        }
                    } else {
                        stack.emplace_back(data);
                    }
                } else {
                    stack.emplace_back(data);
                }
            }
        }

    }

    void addPerimeterLayerLevel(Mesh* mesh, const std::vector<std::pair<ExtrudeEdge, ExtendedFace*>>& edges){
        
        std::vector<PrintLayerLevel>& printLayers = m_structure.printLayers;

        PrintLayerLevel& printLayerLevel = printLayers.emplace_back();
        std::vector<PerimeterOutline>& perimeterOutlines = printLayerLevel.perimeterOutlines;

        std::map<glm::vec3, std::pair<int,std::variant<FrontTag, BackTag>>> pointsHelper;
        glm::vec2 quadTreeMinBounds{ std::numeric_limits<float>::max(), std::numeric_limits<float>::max() };
        glm::vec2 quadTreeMaxBounds{ std::numeric_limits<float>::min(), std::numeric_limits<float>::min() };

        for(auto& [edge, face] : edges) {

            updateQuadtreeBounds(edge, quadTreeMinBounds, quadTreeMaxBounds);

            auto firstPointIt = pointsHelper.find(edge.firstPoint);
            auto secondPointIt = pointsHelper.find(edge.secondPoint);

            if(firstPointIt == pointsHelper.end() &&
                secondPointIt == pointsHelper.end()) {
                PerimeterOutline perimeterOutline;

                perimeterOutline.points.emplace_back(edge, face);

                perimeterOutlines.emplace_back(perimeterOutline);
                
                pointsHelper.emplace(edge.firstPoint, std::make_pair(perimeterOutlines.size()-1, FrontTag{}));
                pointsHelper.emplace(edge.secondPoint, std::make_pair(perimeterOutlines.size()-1, BackTag{}));

            } else if( (firstPointIt == pointsHelper.end() && secondPointIt != pointsHelper.end()) ||
                        (firstPointIt != pointsHelper.end() && secondPointIt == pointsHelper.end())) {
               
                glm::vec3 point = (firstPointIt != pointsHelper.end()) ? edge.secondPoint : edge.firstPoint;

                if(firstPointIt != pointsHelper.end()) {
                    std::pair<int ,std::variant<FrontTag, BackTag>>& indexTagPair = firstPointIt->second;
                    int index = indexTagPair.first;
                    std::variant<FrontTag, BackTag>& tag = indexTagPair.second;

                    if(std::holds_alternative<FrontTag>(tag)) {
                        ExtrudeEdge swappedEdge = edge;
                        swappedEdge.swapPoints();
                        perimeterOutlines.at(index).points.emplace_front(swappedEdge, face);
                        pointsHelper.emplace(point, std::make_pair(index, FrontTag{})); 

                    } else if(std::holds_alternative<BackTag>(tag)) {
                        perimeterOutlines.at(index).points.emplace_back(edge, face);
                        pointsHelper.emplace(point, std::make_pair(index, BackTag{}));
                    }

                    pointsHelper.erase(firstPointIt);

                } else if(secondPointIt != pointsHelper.end()) {
                    std::pair<int ,std::variant<FrontTag, BackTag>>& indexTagPair = secondPointIt->second;
                    int index = indexTagPair.first;
                    std::variant<FrontTag, BackTag>& tag = indexTagPair.second;

                    if(std::holds_alternative<FrontTag>(tag)) {
                        perimeterOutlines.at(index).points.emplace_front(edge, face);
                        pointsHelper.emplace(point, std::make_pair(index, FrontTag{}));

                    } else if(std::holds_alternative<BackTag>(tag)) {
                        ExtrudeEdge swappedEdge = edge;
                        swappedEdge.swapPoints();
                        perimeterOutlines.at(index).points.emplace_back(swappedEdge, face);
                        pointsHelper.emplace(point, std::make_pair(index, BackTag{}));

                    }
                    
                    pointsHelper.erase(secondPointIt);
                }

            } else if(firstPointIt != pointsHelper.end() && secondPointIt != pointsHelper.end()){
                std::pair<int ,std::variant<FrontTag, BackTag>>& firstIndexTagPair = firstPointIt->second;
                std::pair<int ,std::variant<FrontTag, BackTag>>& secondIndexTagPair = secondPointIt->second;
                int firstIndex = firstIndexTagPair.first;
                int secondIndex = secondIndexTagPair.first;

                if(firstIndex == secondIndex) {
                    //uzatvorene
                    //
                    std::variant<FrontTag, BackTag>& firstTag = firstPointIt->second.second;
                    int firstIndex = firstPointIt->second.first;


                    if(std::holds_alternative<FrontTag>(firstTag)) {
                        ExtrudeEdge swappedEdge = edge;
                        swappedEdge.swapPoints();

                        perimeterOutlines.at(firstIndex).points.emplace_front(swappedEdge, face);
                    } else if(std::holds_alternative<BackTag>(firstTag)) {
                        perimeterOutlines.at(firstIndex).points.emplace_back(edge, face);
                    }

                    pointsHelper.erase(firstPointIt);
                    pointsHelper.erase(secondPointIt);
                } else {
                    std::variant<FrontTag, BackTag>& firstTag = firstIndexTagPair.second;
                    std::variant<FrontTag, BackTag>& secondTag = secondIndexTagPair.second;
                    std::list<std::pair<ExtrudeEdge, ExtendedFace*>>& firstList = perimeterOutlines.at(firstIndex).points;
                    std::list<std::pair<ExtrudeEdge, ExtendedFace*>>& secondList = perimeterOutlines.at(secondIndex).points;

                    bool firstFrontTag = std::holds_alternative<FrontTag>(firstTag);
                    bool secondFrontTag = std::holds_alternative<FrontTag>(secondTag);
                    
                    if(firstFrontTag && secondFrontTag){

                        glm::vec3& pointToRemoveFirst = firstList.front().first.firstPoint;
                        glm::vec3& pointToRemoveSecond = secondList.front().first.firstPoint;

                        pointsHelper.erase(pointToRemoveFirst);
                        pointsHelper.erase(pointToRemoveSecond);


                        if(firstIndex == (perimeterOutlines.size()-1)) { 
                            glm::vec3& pointToMove = perimeterOutlines.back().points.back().first.secondPoint;
                            pointsHelper.find(pointToMove)->second.first = secondIndex;
                            pointsHelper.find(pointToMove)->second.second = FrontTag{};
                            
                            for(std::pair<ExtrudeEdge, ExtendedFace*>& currentEdge : firstList) {
                                currentEdge.first.swapPoints(); 
                            }
                            firstList.emplace_front(edge, face);
                            //splice
                            //
                            while (!firstList.empty()) {
                                auto it = firstList.begin();      
                                secondList.splice(secondList.begin(), firstList, it); 
                            }
                            perimeterOutlines.pop_back();

                        } else if(firstIndex != (perimeterOutlines.size()-1)) {
                            glm::vec3& pointToMove = perimeterOutlines.at(firstIndex).points.back().first.secondPoint;
                            pointsHelper.find(pointToMove)->second.first = secondIndex;
                            pointsHelper.find(pointToMove)->second.second = FrontTag{};
                            
                            for(std::pair<ExtrudeEdge, ExtendedFace*>& currentEdge : firstList) {
                                currentEdge.first.swapPoints(); 
                            }
                            firstList.emplace_front(edge, face);
                            //splice
                            while (!firstList.empty()) {
                                auto it = firstList.begin();      
                                secondList.splice(secondList.begin(), firstList, it); 
                            }

                            pointsHelper.find(perimeterOutlines.back().points.front().first.firstPoint)->second.first = firstIndex;
                            pointsHelper.find(perimeterOutlines.back().points.back().first.secondPoint)->second.first = firstIndex;

                            
                            utils::containers::swapLastAndPop(perimeterOutlines, firstIndex);
                        }

                    } else if((firstFrontTag && !secondFrontTag) ||
                            (!firstFrontTag && secondFrontTag) )
                    {
                        if(!firstFrontTag) {
                            glm::vec3& pointToRemoveFirst = firstList.back().first.secondPoint;
                            glm::vec3& pointToRemoveSecond = secondList.front().first.firstPoint;

                            pointsHelper.erase(pointToRemoveFirst);
                            pointsHelper.erase(pointToRemoveSecond);
                            //tam kde teraz nic nie je je firstList
                            //to znamena, ze si zobereme index pre firstList
                            //firstIndex
                            if(firstIndex == (perimeterOutlines.size()-1)) {
                                glm::vec3& pointToMove = perimeterOutlines.at(firstIndex).points.front().first.firstPoint;
                                pointsHelper.find(pointToMove)->second.first = secondIndex;
                               
                                firstList.emplace_back(edge, face);
                                secondList.splice(secondList.begin(), firstList);

                                perimeterOutlines.pop_back();

                            } else if(firstIndex != (perimeterOutlines.size()-1)) {
                                //firstIndex nie je na konci
                                glm::vec3& pointToMove = perimeterOutlines.at(firstIndex).points.front().first.firstPoint;
                                pointsHelper.find(pointToMove)->second.first = secondIndex;
                               
                                firstList.emplace_back(edge, face);
                                secondList.splice(secondList.begin(), firstList);
                                //move with the last element
                                //change ids of last element
                                pointsHelper.find(perimeterOutlines.back().points.front().first.firstPoint)->second.first = firstIndex;
                                pointsHelper.find(perimeterOutlines.back().points.back().first.secondPoint)->second.first = firstIndex;

                                utils::containers::swapLastAndPop(perimeterOutlines, firstIndex);
                            }
                        } else {
                            //TERAZ SPRAVIT TOTO!!!
                            glm::vec3& pointToRemoveSecond = secondList.back().first.secondPoint;
                            glm::vec3& pointToRemoveFirst = firstList.front().first.firstPoint;

                            pointsHelper.erase(pointToRemoveSecond);
                            pointsHelper.erase(pointToRemoveFirst);

                            if(firstIndex == (perimeterOutlines.size() - 1)) {
                                glm::vec3& pointToMove = perimeterOutlines.at(firstIndex).points.back().first.secondPoint;
                                pointsHelper.find(pointToMove)->second.first = secondIndex;
                                
                                firstList.emplace_front(edge, face);
                                firstList.front().first.swapPoints();

                                secondList.splice(secondList.end(), firstList);

                                perimeterOutlines.pop_back();

                            } else if(firstIndex != (perimeterOutlines.size()-1)) {
                                glm::vec3& pointToMove = perimeterOutlines.at(firstIndex).points.back().first.secondPoint;
                                pointsHelper.find(pointToMove)->second.first = secondIndex;
                                
                                firstList.emplace_front(edge, face);
                                firstList.front().first.swapPoints();

                                secondList.splice(secondList.end(), firstList);

                                pointsHelper.find(perimeterOutlines.back().points.front().first.firstPoint)->second.first = firstIndex;
                                pointsHelper.find(perimeterOutlines.back().points.back().first.secondPoint)->second.first = firstIndex;

                                utils::containers::swapLastAndPop(perimeterOutlines, firstIndex);
                            }


                        }
                    } else if(!firstFrontTag && !secondFrontTag) {
                       
                        glm::vec3& pointToRemoveFirst = firstList.back().first.secondPoint;
                        glm::vec3& pointToRemoveSecond = secondList.back().first.secondPoint;

                        pointsHelper.erase(pointToRemoveFirst);
                        pointsHelper.erase(pointToRemoveSecond);


                        if(firstIndex == (perimeterOutlines.size()-1)) { 
                            glm::vec3& pointToMove = perimeterOutlines.at(firstIndex).points.front().first.firstPoint;
                            pointsHelper.find(pointToMove)->second.first = secondIndex;
                            pointsHelper.find(pointToMove)->second.second = BackTag{};


                            firstList.emplace_back(edge, face);
                            for(std::pair<ExtrudeEdge, ExtendedFace*>& currentEdge : firstList) {
                                currentEdge.first.swapPoints(); 
                            }

                            //splice
                            while (!firstList.empty()) {
                                // Iterator to the last element
                                auto it = std::prev(firstList.end());
                                // Move (splice) last element from first → back of second
                                secondList.splice(secondList.end(), firstList, it);
                            }

                            perimeterOutlines.pop_back();

                        } else if(firstIndex != (perimeterOutlines.size()-1)) {
                            glm::vec3& pointToMove = perimeterOutlines.at(firstIndex).points.front().first.firstPoint;
                            pointsHelper.find(pointToMove)->second.first = secondIndex;
                            pointsHelper.find(pointToMove)->second.second = BackTag{};


                            firstList.emplace_back(edge, face);
                            for(std::pair<ExtrudeEdge, ExtendedFace*>& currentEdge : firstList) {
                                currentEdge.first.swapPoints(); 
                            }

                            //splice
                            while (!firstList.empty()) {
                                // Iterator to the last element
                                auto it = std::prev(firstList.end());
                                // Move (splice) last element from first → back of second
                                secondList.splice(secondList.end(), firstList, it);
                            }

                            pointsHelper.find(perimeterOutlines.back().points.front().first.firstPoint)->second.first = firstIndex;
                            pointsHelper.find(perimeterOutlines.back().points.back().first.secondPoint)->second.first = firstIndex;

                            
                            utils::containers::swapLastAndPop(perimeterOutlines, firstIndex);
                        }

                    }

                }
            }
        }
        glm::vec2 minQuadtreeBounds{mesh->m_meshBounds.getMinBoundsPos().x, mesh->m_meshBounds.getMinBoundsPos().z};
        glm::vec2 maxQuadtreeBounds{mesh->m_meshBounds.getMaxBoundsPos().x, mesh->m_meshBounds.getMaxBoundsPos().z};
        
        Quadtree<OutlinerEdgeHelperData> outlinerQuadtree(minQuadtreeBounds, maxQuadtreeBounds);

        //nakoniec sa musi prejst cez vsetky vytvorene edges
        //potrebujem kazdemu 
        //chod cez kazdy jeden outline, projectni tie vertices na 2D vertices.
        int outlineId = 0;
        float yTemp = 0.0f;
        bool yTempInitialized = false;
        for(PerimeterOutline& outline : perimeterOutlines) {
            std::vector<glm::vec2> outline2DProjection;
            for(auto& [extrudeEdge, _] : outline.points) {

                if(!yTempInitialized){
                    yTemp = extrudeEdge.firstPoint.y;
                    yTempInitialized = true;
                }
              
                glm::vec2 projected2DPoint(extrudeEdge.firstPoint.x, extrudeEdge.firstPoint.z);
                outline2DProjection.emplace_back(projected2DPoint);
            }
            
            //dobre teraz mame spravenu 2D, musime zavolat metodu, ktora mi povie o tom ci je polygon CCW alebo CW
            utils::geometry::WindingOrder windingOrder = utils::geometry::getWindingOrder(outline2DProjection);
            outline.windingOrder = windingOrder;
          
            //POTOM TOTO DAT NASPAT
            
            //CREATING A PLANE FROM TWO DIRECTIONS
            glm::vec3 firstOutlinePoint = outline.points.begin()->first.firstPoint;
            glm::vec3 secondOutlinePoint = outline.points.begin()->first.secondPoint;
            glm::vec3 outlineDir = glm::normalize(secondOutlinePoint - firstOutlinePoint);

            glm::vec3 upDir{ 0.0f, 1.0f, 0.0f };
            glm::vec3 planeNormal = glm::normalize(glm::cross(upDir, outlineDir));
            
            //PLANE DONE
            //
            //GET NORMAL OF FACE 
            ExtendedFace* face = outline.points.begin()->second;
            glm::vec3 faceNormal = utils::geometry::computePolygonNormal(face);

            glm::vec2 outlinePerpendicular2DDir = utils::geometry::getInwardNormal({firstOutlinePoint.x, firstOutlinePoint.z}, { secondOutlinePoint.x, secondOutlinePoint.z} , outline.windingOrder);
            glm::vec3 outlinePerpendicular3DDir{ outlinePerpendicular2DDir.x, 0.0f, outlinePerpendicular2DDir.y };
            
           
            utils::geometry::SideRelation sideRelation = utils::geometry::checkSideRelation(planeNormal, outlinePerpendicular3DDir, faceNormal);
            if(sideRelation == utils::geometry::SideRelation::OppositeSide) {
                outline.filled = true;
            } else if(sideRelation == utils::geometry::SideRelation::SameSide) {
                outline.filled = false;
            }

            for(auto it = outline.points.begin(); it != outline.points.end(); ++it) {
                OutlinerEdgeHelperData data(it->first.firstPoint, it->first.secondPoint, outline.filled, outlineId);
                glm::vec2 minDataBounds = glm::min(glm::vec2(it->first.firstPoint.x, it->first.firstPoint.z), glm::vec2(it->first.secondPoint.x, it->first.secondPoint.z));
                glm::vec2 maxDataBounds = glm::max(glm::vec2(it->first.firstPoint.x, it->first.firstPoint.z), glm::vec2(it->first.secondPoint.x, it->first.secondPoint.z));
                BoundingRegion2D dataBounds(minDataBounds, maxDataBounds);
                outlinerQuadtree.addDataToQuadtree(data, dataBounds);
            }

            ++outlineId;
        }

        //dobre mame vybudovany quadtree uz, teraz mozme s tymto quadtree zavolat metodu
        //ktora mi zase naopak vytvori vsetky potrebne infill layers
        addInfillLayerLevel(outlinerQuadtree, yTemp);

    }

public:
    PrintableMeshStructure m_structure;
};
