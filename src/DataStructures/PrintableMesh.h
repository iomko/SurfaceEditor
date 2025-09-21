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

    //testing part
    bool filled = false;
    bool highlightTest = false;
    
    glm::vec3 faceNormalEdge{};
    glm::vec3 planeNormalEdge{};
    bool hasPerpendicular = false;
    glm::vec3 perpendicularEdge{};
};

struct PerimeterOutline {
    std::list<std::pair<ExtrudePoint, ExtendedFace*>> points;
    utils::geometry::WindingOrder windingOrder = utils::geometry::WindingOrder::None;
    bool filled = false;
};

struct InfillLine {
    std::list<ExtrudePoint> points;
};

struct PrintLayerLevel {
    //using InfillLine = std::list<ExtrudePoint>;
    //using PerimeterOutline = std::list<std::pair<ExtrudePoint, ExtendedFace*>>;

    //using InfillLayerLevel = std::vector<InfillLine>;
    //using PerimeterLayerLevel = std::vector<PerimeterOutline>;

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
        float incrementNum = 10.0f;
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

    void addPerimeterLayerLevel(std::vector<std::pair<ExtrudeEdge, ExtendedFace*>>& edges){
        std::vector<PrintLayerLevel>& printLayers = m_structure.printLayers;

        PrintLayerLevel& printLayerLevel = printLayers.emplace_back();
        std::vector<PerimeterOutline>& perimeterOutlines = printLayerLevel.perimeterOutlines;
        //PrintLayerLevel::PerimeterLayerLevel& perimeterLayerLevel = printLayerLevel.perimeterLayerLevel;

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
                //PrintLayerLevel::PerimeterOutline perimeterOutline;
                perimeterOutline.points.emplace_back(edge.firstPoint, face);
                perimeterOutline.points.emplace_back(edge.secondPoint, face);

                perimeterOutlines.emplace_back(perimeterOutline);
                
                pointsHelper.emplace(edge.firstPoint, std::make_pair(perimeterOutlines.size()-1, FrontTag{}));
                pointsHelper.emplace(edge.secondPoint, std::make_pair(perimeterOutlines.size()-1, BackTag{}));

            } else if( (firstPointIt == pointsHelper.end() && secondPointIt != pointsHelper.end()) ||
                        (firstPointIt != pointsHelper.end() && secondPointIt == pointsHelper.end())) {
               
                glm::vec3 point = (firstPointIt != pointsHelper.end()) ? edge.secondPoint : edge.firstPoint;
                auto it = (firstPointIt != pointsHelper.end()) ? firstPointIt : secondPointIt;
                std::pair<int ,std::variant<FrontTag, BackTag>>& indexTagPair = it->second;
                int index = indexTagPair.first;
                std::variant<FrontTag, BackTag>& tag = indexTagPair.second;

                if(std::holds_alternative<FrontTag>(tag)){
                    perimeterOutlines.at(index).points.emplace_front(point, face);
                    pointsHelper.emplace(point, std::make_pair(index, FrontTag{}));

                } else if(std::holds_alternative<BackTag>(tag)){
                    perimeterOutlines.at(index).points.emplace_back(point, face);
                    pointsHelper.emplace(point, std::make_pair(index, BackTag{}));

                }
                pointsHelper.erase(it);
            } else if(firstPointIt != pointsHelper.end() && secondPointIt != pointsHelper.end()){
                std::pair<int ,std::variant<FrontTag, BackTag>>& firstIndexTagPair = firstPointIt->second;
                std::pair<int ,std::variant<FrontTag, BackTag>>& secondIndexTagPair = secondPointIt->second;
                int firstIndex = firstIndexTagPair.first;
                int secondIndex = secondIndexTagPair.first;

                if(firstIndex == secondIndex) {
                    //uzatvorene
                    pointsHelper.erase(firstPointIt);
                    pointsHelper.erase(secondPointIt);
                } else {
                    std::variant<FrontTag, BackTag>& firstTag = firstIndexTagPair.second;
                    std::variant<FrontTag, BackTag>& secondTag = secondIndexTagPair.second;
                    std::list<std::pair<ExtrudePoint, ExtendedFace*>>& firstList = perimeterOutlines.at(firstIndex).points;
                    std::list<std::pair<ExtrudePoint, ExtendedFace*>>& secondList = perimeterOutlines.at(secondIndex).points;

                    bool firstFrontTag = std::holds_alternative<FrontTag>(firstTag);
                    bool secondFrontTag = std::holds_alternative<FrontTag>(secondTag);
                    
                    if(firstFrontTag && secondFrontTag){

                        ExtrudePoint& extrudePointToRemoveFirst = firstList.front().first;
                        ExtrudePoint& extrudePointToRemoveSecond = secondList.front().first;

                        pointsHelper.erase(extrudePointToRemoveFirst.pos);
                        pointsHelper.erase(extrudePointToRemoveSecond.pos);


                        if(firstIndex == (perimeterOutlines.size()-1)) { 
                            ExtrudePoint& extrudePointToMove = perimeterOutlines.back().points.back().first;
                            pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;
                            pointsHelper.find(extrudePointToMove.pos)->second.second = FrontTag{};

                            //splice
                            //
                            while (!firstList.empty()) {
                                auto it = firstList.begin();      
                                secondList.splice(secondList.begin(), firstList, it); 
                            }
                            perimeterOutlines.pop_back();

                        } else if(firstIndex != (perimeterOutlines.size()-1)) {
                            ExtrudePoint& extrudePointToMove = perimeterOutlines.at(firstIndex).points.back().first;
                            pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;
                            pointsHelper.find(extrudePointToMove.pos)->second.second = FrontTag{};

                            //splice
                            while (!firstList.empty()) {
                                auto it = firstList.begin();      
                                secondList.splice(secondList.begin(), firstList, it); 
                            }

                            pointsHelper.find(perimeterOutlines.back().points.front().first.pos)->second.first = firstIndex;
                            pointsHelper.find(perimeterOutlines.back().points.back().first.pos)->second.first = firstIndex;

                            
                            utils::containers::swapLastAndPop(perimeterOutlines, firstIndex);
                        }

                    } else if((firstFrontTag && !secondFrontTag) ||
                            (!firstFrontTag && secondFrontTag) )
                    {
                        if(!firstFrontTag) {
                            ExtrudePoint& extrudePointToRemoveFirst = firstList.back().first;
                            ExtrudePoint& extrudePointToRemoveSecond = secondList.front().first;

                            pointsHelper.erase(extrudePointToRemoveFirst.pos);
                            pointsHelper.erase(extrudePointToRemoveSecond.pos);
                            //tam kde teraz nic nie je je firstList
                            //to znamena, ze si zobereme index pre firstList
                            //firstIndex
                            if(firstIndex == (perimeterOutlines.size()-1)) {
                                ExtrudePoint& extrudePointToMove = perimeterOutlines.back().points.front().first;
                                pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;

                                secondList.splice(secondList.begin(), firstList);
                                perimeterOutlines.pop_back();

                            } else if(firstIndex != (perimeterOutlines.size()-1)) {
                                //firstIndex nie je na konci
                                ExtrudePoint& extrudePointToMove = perimeterOutlines.at(firstIndex).points.front().first;
                                pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;
                               

                                secondList.splice(secondList.begin(), firstList);
                                //move with the last element
                                //change ids of last element
                                pointsHelper.find(perimeterOutlines.back().points.front().first.pos)->second.first = firstIndex;
                                pointsHelper.find(perimeterOutlines.back().points.back().first.pos)->second.first = firstIndex;

                                utils::containers::swapLastAndPop(perimeterOutlines, firstIndex);
                            }
                        } else {
                            //TERAZ SPRAVIT TOTO!!!
                            ExtrudePoint& extrudePointToRemoveSecond = secondList.back().first;
                            ExtrudePoint& extrudePointToRemoveFirst = firstList.front().first;

                            pointsHelper.erase(extrudePointToRemoveSecond.pos);
                            pointsHelper.erase(extrudePointToRemoveFirst.pos);

                            if(firstIndex == (perimeterOutlines.size() - 1)) {
                                ExtrudePoint& extrudePointToMove = perimeterOutlines.back().points.back().first;
                                pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;

                                secondList.splice(secondList.end(), firstList);
                                perimeterOutlines.pop_back();

                            } else if(firstIndex != (perimeterOutlines.size()-1)) {
                                ExtrudePoint& extrudePointToMove = perimeterOutlines.at(firstIndex).points.back().first;
                                pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;

                                secondList.splice(secondList.end(), firstList);

                                pointsHelper.find(perimeterOutlines.back().points.front().first.pos)->second.first = firstIndex;
                                pointsHelper.find(perimeterOutlines.back().points.back().first.pos)->second.first = firstIndex;

                                utils::containers::swapLastAndPop(perimeterOutlines, firstIndex);
                            }


                        }
                    } else if(!firstFrontTag && !secondFrontTag) {
                       
                        ExtrudePoint& extrudePointToRemoveFirst = firstList.back().first;
                        ExtrudePoint& extrudePointToRemoveSecond = secondList.back().first;

                        pointsHelper.erase(extrudePointToRemoveFirst.pos);
                        pointsHelper.erase(extrudePointToRemoveSecond.pos);


                        if(firstIndex == (perimeterOutlines.size()-1)) { 
                            ExtrudePoint& extrudePointToMove = perimeterOutlines.back().points.front().first;
                            pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;
                            pointsHelper.find(extrudePointToMove.pos)->second.second = BackTag{};

                            //splice
                            while (!firstList.empty()) {
                                // Iterator to the last element
                                auto it = std::prev(firstList.end());
                                // Move (splice) last element from first → back of second
                                secondList.splice(secondList.end(), firstList, it);
                            }
                            perimeterOutlines.pop_back();

                        } else if(firstIndex != (perimeterOutlines.size()-1)) {
                            ExtrudePoint& extrudePointToMove = perimeterOutlines.at(firstIndex).points.front().first;
                            pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;
                            pointsHelper.find(extrudePointToMove.pos)->second.second = BackTag{};

                            //splice
                            while (!firstList.empty()) {
                                // Iterator to the last element
                                auto it = std::prev(firstList.end());
                                // Move (splice) last element from first → back of second
                                secondList.splice(secondList.end(), firstList, it);
                            }

                            pointsHelper.find(perimeterOutlines.back().points.front().first.pos)->second.first = firstIndex;
                            pointsHelper.find(perimeterOutlines.back().points.back().first.pos)->second.first = firstIndex;

                            
                            utils::containers::swapLastAndPop(perimeterOutlines, firstIndex);
                        }

                    }

                }
            }
        }
        
        Quadtree<OutlinerEdgeHelperData> outlinerQuadtree(quadTreeMinBounds, quadTreeMaxBounds);

        //nakoniec sa musi prejst cez vsetky vytvorene edges
        //potrebujem kazdemu 
        //chod cez kazdy jeden outline, projectni tie vertices na 2D vertices.
        int outlineId = 0;
        float yTemp = 0.0f;
        bool yTempInitialized = false;
        for(PerimeterOutline& outline : perimeterOutlines) {
            std::vector<glm::vec2> outline2DProjection;
            for(auto& [extrudePoint, _] : outline.points) {

                if(!yTempInitialized){
                    yTemp = extrudePoint.pos.y;
                    yTempInitialized = true;
                }

                outline2DProjection.emplace_back(extrudePoint.pos.x, extrudePoint.pos.z);
            }
            
            //dobre teraz mame spravenu 2D, musime zavolat metodu, ktora mi povie o tom ci je polygon CCW alebo CW
            utils::geometry::WindingOrder windingOrder = utils::geometry::getWindingOrder(outline2DProjection);
            outline.windingOrder = windingOrder;
          
            //POTOM TOTO DAT NASPAT
            /*
            //CREATING A PLANE FROM TWO DIRECTIONS
            glm::vec3 firstOutlinePoint = outline.points.begin()->first.pos;
            glm::vec3 secondOutlinePoint = std::next(outline.points.begin())->first.pos;
            glm::vec3 outlineDir = glm::normalize(firstOutlinePoint - secondOutlinePoint);

            glm::vec3 upDir{ 0.0f, 1.0f, 0.0f };
            glm::vec3 planeNormal = glm::normalize(glm::cross(upDir, outlineDir));
            
            //PLANE DONE
            //
            //GET NORMAL OF FACE 
            ExtendedFace* face = outline.points.begin()->second;
            glm::vec3 faceNormal = utils::geometry::computePolygonNormal(face);

            glm::vec2 outlinePerpendicular2DDir = utils::geometry::getRightNormal({firstOutlinePoint.x, firstOutlinePoint.z}, { secondOutlinePoint.x, secondOutlinePoint.z} );
            if(outline.windingOrder == utils::geometry::WindingOrder::CCW) {
                -outlinePerpendicular2DDir;
            }
            glm::vec3 outlinePerpendicular3DDir{ outlinePerpendicular2DDir.x, firstOutlinePoint.y, outlinePerpendicular2DDir.y };
            
            //tak mame teraz k dispozicii
            //planeNormal
            //outlinePerpendicular3DDir
            //faceNormal
            //
           
            utils::geometry::SideRelation sideRelation = utils::geometry::checkSideRelation(planeNormal, outlinePerpendicular3DDir, faceNormal);
            if(sideRelation == utils::geometry::SideRelation::OppositeSide) {
                outline.filled = true;
            } else if(sideRelation == utils::geometry::SideRelation::SameSide) {
                outline.filled = false;
            }
            */



            //dobre teraz uz kazdy perimeterOutline ma nastavene to ze ci ma byt filled alebo nie
            //
            //teraz musime dalej pridat tento outline, ale len 2D do quadTree, na to aby som mohol rychlo vyhladat pomocou ray intersection
            //danu line
 
            for(auto it = outline.points.begin(); it != outline.points.end(); ++it) {
                auto nextIt = std::next(it);
                if(nextIt == outline.points.end()) {
                    nextIt = outline.points.begin();
                }

                ExtendedFace* face = it->second;
                glm::vec3 faceNormal = utils::geometry::computePolygonNormal(face);

                glm::vec3 firstEdgePoint = it->first.pos;
                glm::vec3 secondEdgePoint = nextIt->first.pos;
                glm::vec3 edgeDir = glm::normalize(firstEdgePoint - secondEdgePoint);

                glm::vec3 planeUpDir{ 0.0f, 1.0f, 0.0f };
                glm::vec3 planeNormal = glm::normalize(glm::cross(planeUpDir, edgeDir));

                glm::vec2 outlinePerpendicularDir = utils::geometry::getRightNormal({firstEdgePoint.x, firstEdgePoint.z}, { secondEdgePoint.x, secondEdgePoint.z} );
                if(outline.windingOrder == utils::geometry::WindingOrder::CCW) {
                    outlinePerpendicularDir = -outlinePerpendicularDir;
                }
                glm::vec3 outline3DPerpendicularDir{ outlinePerpendicularDir.x, firstEdgePoint.y, outlinePerpendicularDir.y };

                utils::geometry::SideRelation sideRelation = utils::geometry::checkSideRelation(planeNormal, outline3DPerpendicularDir, faceNormal);
                if(sideRelation == utils::geometry::SideRelation::OppositeSide) {
                    it->first.filled = true;
                    nextIt->first.filled = true;
                    it->first.perpendicularEdge = outline3DPerpendicularDir;
                    it->first.hasPerpendicular = true;
                } else if(sideRelation == utils::geometry::SideRelation::SameSide) {
                    it->first.filled = false;
                    nextIt->first.filled = false;
                    
                    if(!infillAlreadySet) {
                        it->first.highlightTest = true;
                        nextIt->first.highlightTest = true;

                        it->first.faceNormalEdge = faceNormal;
                        it->first.planeNormalEdge = planeNormal;
                        it->first.perpendicularEdge = outline3DPerpendicularDir;
                        std::cout << "---NOT AN INFILL---" << std::endl;
                        std::cout << "FaceNormal: " << faceNormal.x << ", " << faceNormal.y << ", " << faceNormal.z << std::endl;
                        std::cout << "planeNormal: " << planeNormal.x << ", " << planeNormal.y << ", " << planeNormal.z << std::endl;
                        std::cout << "firstPoint: " << firstEdgePoint.x << ", " << firstEdgePoint.y << ", " << firstEdgePoint.z << std::endl;
                        std::cout << "secondPoint: " << secondEdgePoint.x << ", " << secondEdgePoint.y << ", " << secondEdgePoint.z << std::endl;
                        if(outline.windingOrder == utils::geometry::WindingOrder::CCW) {
                            std::cout << "WindingOrder: CCW" << std::endl;
                        } else if (outline.windingOrder == utils::geometry::WindingOrder::CW) {
                            std::cout << "WindingOrder: CW" << std::endl;
                        } else if(outline.windingOrder == utils::geometry::WindingOrder::None) {
                            std::cout << "WindingOrder: None" << std::endl; 
                        }
                        std::cout << "PerpendicularEdge: " << outline3DPerpendicularDir.x << ", " << outline3DPerpendicularDir.y << ", " << outline3DPerpendicularDir.z << std::endl;
                        infillAlreadySet = true;
                    }

                }


                //musime vytvorit data ktore sa budu vkladat do quadtree
                OutlinerEdgeHelperData data(it->first.pos, nextIt->first.pos, outline.filled, outlineId);
                glm::vec2 minDataBounds = glm::min(glm::vec2(it->first.pos.x, it->first.pos.z), glm::vec2(nextIt->first.pos.x, nextIt->first.pos.z));
                glm::vec2 maxDataBounds = glm::max(glm::vec2(it->first.pos.x, it->first.pos.z), glm::vec2(nextIt->first.pos.x, nextIt->first.pos.z));
                BoundingRegion2D dataBounds(minDataBounds, maxDataBounds);
                outlinerQuadtree.addDataToQuadtree(data, dataBounds);
            }
            ++outlineId;
        }

        //dobre mame vybudovany quadtree uz, teraz mozme s tymto quadtree zavolat metodu
        //ktora mi zase naopak vytvori vsetky potrebne infill layers
        //addInfillLayerLevel(outlinerQuadtree, yTemp);

    }

public:
    PrintableMeshStructure m_structure;
    bool infillAlreadySet = false;
};
