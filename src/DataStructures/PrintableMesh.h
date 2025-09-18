#pragma once
#include <vector>
#include <list>
#include <glm/glm.hpp>
#include <variant>
#include <map>
#include "Utils/ContainerUtils.h"


struct ExtrudeEdge {
    ExtrudeEdge(const glm::vec3 firstPoint, const glm::vec3 secondPoint) {
        this->firstPoint = firstPoint;
        this->secondPoint = secondPoint;
    }
    glm::vec3 firstPoint;
    glm::vec3 secondPoint;
};

struct ExtrudePoint {
    glm::vec3 pos;
};

struct PrintLayer {
    std::vector<std::list<ExtrudePoint>> levelLayers;
};

class PrintableMesh {
private:
    struct FrontTag {};
    struct BackTag {};
public:
    PrintableMesh() {

    }

    void removeAllLevelLayers() {
        m_perimeterLayer.clear();
    }

    void addLevelLayers(const std::vector<ExtrudeEdge>& edges){
        PrintLayer& printLayer = m_perimeterLayer.emplace_back();
        std::vector<std::list<ExtrudePoint>>& levelLayers = printLayer.levelLayers;

        std::map<glm::vec3, std::pair<int,std::variant<FrontTag, BackTag>>> pointsHelper;
            
        for(const ExtrudeEdge& edge : edges) {
            auto firstPointIt = pointsHelper.find(edge.firstPoint);
            auto secondPointIt = pointsHelper.find(edge.secondPoint);

            if(firstPointIt == pointsHelper.end() &&
               secondPointIt == pointsHelper.end()) {
                
                levelLayers.emplace_back(std::list<ExtrudePoint>{{edge.firstPoint}, {edge.secondPoint}});
                
                pointsHelper.emplace(edge.firstPoint, std::make_pair(levelLayers.size()-1, FrontTag{}));
                pointsHelper.emplace(edge.secondPoint, std::make_pair(levelLayers.size()-1, BackTag{}));

            } else if( (firstPointIt == pointsHelper.end() && secondPointIt != pointsHelper.end()) ||
                        (firstPointIt != pointsHelper.end() && secondPointIt == pointsHelper.end())) {
               
                glm::vec3 point = (firstPointIt != pointsHelper.end()) ? edge.secondPoint : edge.firstPoint;
                auto it = (firstPointIt != pointsHelper.end()) ? firstPointIt : secondPointIt;
                std::pair<int ,std::variant<FrontTag, BackTag>>& indexTagPair = it->second;
                int index = indexTagPair.first;
                std::variant<FrontTag, BackTag>& tag = indexTagPair.second;

                if(std::holds_alternative<FrontTag>(tag)){
                    levelLayers.at(index).emplace_front(point);
                    pointsHelper.emplace(point, std::make_pair(index, FrontTag{}));

                } else if(std::holds_alternative<BackTag>(tag)){
                    levelLayers.at(index).emplace_back(point);
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
                    std::list<ExtrudePoint>& firstList = levelLayers.at(firstIndex);
                    std::list<ExtrudePoint>& secondList = levelLayers.at(secondIndex);

                    bool firstFrontTag = std::holds_alternative<FrontTag>(firstTag);
                    bool secondFrontTag = std::holds_alternative<FrontTag>(secondTag);
                    
                    if(firstFrontTag && secondFrontTag){

                        ExtrudePoint& extrudePointToRemoveFirst = firstList.front();
                        ExtrudePoint& extrudePointToRemoveSecond = secondList.front();

                        pointsHelper.erase(extrudePointToRemoveFirst.pos);
                        pointsHelper.erase(extrudePointToRemoveSecond.pos);


                        if(firstIndex == (levelLayers.size()-1)) { 
                            ExtrudePoint& extrudePointToMove = levelLayers.back().back();
                            pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;
                            pointsHelper.find(extrudePointToMove.pos)->second.second = FrontTag{};

                            //splice
                            //
                            while (!firstList.empty()) {
                                auto it = firstList.begin();      
                                secondList.splice(secondList.begin(), firstList, it); 
                            }
                            levelLayers.pop_back();

                        } else if(firstIndex != (levelLayers.size()-1)) {
                            ExtrudePoint& extrudePointToMove = levelLayers.at(firstIndex).back();
                            pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;
                            pointsHelper.find(extrudePointToMove.pos)->second.second = FrontTag{};

                            //splice
                            while (!firstList.empty()) {
                                auto it = firstList.begin();      
                                secondList.splice(secondList.begin(), firstList, it); 
                            }

                            pointsHelper.find(levelLayers.back().front().pos)->second.first = firstIndex;
                            pointsHelper.find(levelLayers.back().back().pos)->second.first = firstIndex;

                            
                            utils::containers::swapLastAndPop(levelLayers, firstIndex);
                        }

                    } else if((firstFrontTag && !secondFrontTag) ||
                            (!firstFrontTag && secondFrontTag) )
                    {
                        if(!firstFrontTag) {
                            ExtrudePoint& extrudePointToRemoveFirst = firstList.back();
                            ExtrudePoint& extrudePointToRemoveSecond = secondList.front();

                            pointsHelper.erase(extrudePointToRemoveFirst.pos);
                            pointsHelper.erase(extrudePointToRemoveSecond.pos);
                            //tam kde teraz nic nie je je firstList
                            //to znamena, ze si zobereme index pre firstList
                            //firstIndex
                            if(firstIndex == (levelLayers.size()-1)) {
                                ExtrudePoint& extrudePointToMove = levelLayers.back().front();
                                pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;

                                secondList.splice(secondList.begin(), firstList);
                                levelLayers.pop_back();

                            } else if(firstIndex != (levelLayers.size()-1)) {
                                //firstIndex nie je na konci
                                ExtrudePoint& extrudePointToMove = levelLayers.at(firstIndex).front();
                                pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;
                               

                                secondList.splice(secondList.begin(), firstList);
                                //move with the last element
                                //change ids of last element
                                pointsHelper.find(levelLayers.back().front().pos)->second.first = firstIndex;
                                pointsHelper.find(levelLayers.back().back().pos)->second.first = firstIndex;

                                utils::containers::swapLastAndPop(levelLayers, firstIndex);
                            }
                        } else {
                            //TERAZ SPRAVIT TOTO!!!
                            ExtrudePoint& extrudePointToRemoveSecond = secondList.back();
                            ExtrudePoint& extrudePointToRemoveFirst = firstList.front();

                            pointsHelper.erase(extrudePointToRemoveSecond.pos);
                            pointsHelper.erase(extrudePointToRemoveFirst.pos);

                            if(firstIndex == (levelLayers.size() - 1)) {
                                ExtrudePoint& extrudePointToMove = levelLayers.back().back();
                                pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;

                                secondList.splice(secondList.end(), firstList);
                                levelLayers.pop_back();

                            } else if(firstIndex != (levelLayers.size()-1)) {
                                ExtrudePoint& extrudePointToMove = levelLayers.at(firstIndex).back();
                                pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;

                                secondList.splice(secondList.end(), firstList);

                                pointsHelper.find(levelLayers.back().front().pos)->second.first = firstIndex;
                                pointsHelper.find(levelLayers.back().back().pos)->second.first = firstIndex;

                                utils::containers::swapLastAndPop(levelLayers, firstIndex);
                            }


                        }
                    } else if(!firstFrontTag && !secondFrontTag) {
                       
                        ExtrudePoint& extrudePointToRemoveFirst = firstList.back();
                        ExtrudePoint& extrudePointToRemoveSecond = secondList.back();

                        pointsHelper.erase(extrudePointToRemoveFirst.pos);
                        pointsHelper.erase(extrudePointToRemoveSecond.pos);


                        if(firstIndex == (levelLayers.size()-1)) { 
                            ExtrudePoint& extrudePointToMove = levelLayers.back().front();
                            pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;
                            pointsHelper.find(extrudePointToMove.pos)->second.second = BackTag{};

                            //splice
                            while (!firstList.empty()) {
                                // Iterator to the last element
                                auto it = std::prev(firstList.end());
                                // Move (splice) last element from first → back of second
                                secondList.splice(secondList.end(), firstList, it);
                            }
                            levelLayers.pop_back();

                        } else if(firstIndex != (levelLayers.size()-1)) {
                            ExtrudePoint& extrudePointToMove = levelLayers.at(firstIndex).front();
                            pointsHelper.find(extrudePointToMove.pos)->second.first = secondIndex;
                            pointsHelper.find(extrudePointToMove.pos)->second.second = BackTag{};

                            //splice
                            while (!firstList.empty()) {
                                // Iterator to the last element
                                auto it = std::prev(firstList.end());
                                // Move (splice) last element from first → back of second
                                secondList.splice(secondList.end(), firstList, it);
                            }

                            pointsHelper.find(levelLayers.back().front().pos)->second.first = firstIndex;
                            pointsHelper.find(levelLayers.back().back().pos)->second.first = firstIndex;

                            
                            utils::containers::swapLastAndPop(levelLayers, firstIndex);
                        }

                    }

                }
            }
        }
    }

public:
    std::vector<PrintLayer> m_perimeterLayer;
};
