#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "../Callback.h"
#include "../../ViewPortsHolder.h"
#include "../../AABBBoundingRegion.h"

class SelectVertexCallBack : public Callback
{
public:

    virtual void execute() override
    {
        std::cout << "SELECTVERTEXCALLBACK" << std::endl;
        
    }
    

private:
    std::pair<bool, float> isPointInsideSphere(const glm::vec3& point, const glm::vec3& sphereCenter, float sphereRadius) {
        float distance = glm::distance(point, sphereCenter);
        bool isInside = (distance <= sphereRadius);
        return std::make_pair(isInside, distance);
    }

    std::pair<std::vector<HalfEdgeDS::VertexIndex>, std::vector<HalfEdgeDS::FaceIndex>> accessNeighbouringVertsAndFacesOfSelectedVertex(HalfEdgeDS::Vertex* vertex)
    {
        HalfEdgeDS::HalfEdge* startingHalfEdge = &(*vertex->getHalfEdge());
        HalfEdgeDS::HalfEdge* currentHalfEdge = startingHalfEdge;
        bool previouslyAccessedNext = true;
        bool previouslyVisitedBoundaryEdge = false;
        bool visitedTwoBoundaryEdges = false;
        std::vector<HalfEdgeDS::FaceIndex> returnedFacesIndices;
        std::vector<HalfEdgeDS::VertexIndex> returnedVerticesIndices;
        //ak sme predtym accessli Next tak to bude TRUE
        //ak sme predtym accessli Previous tak to bude FALSE

        do {
            returnedFacesIndices.push_back(currentHalfEdge->getFaceIndex());
            //returnedFaces.push_back(&(*currentHalfEdge->getFace()));

            if ((*currentHalfEdge->getVertex()) == *vertex)
            {
                returnedVerticesIndices.push_back((*currentHalfEdge->getNext()).getVertexIndex());
                //returnedVertices.push_back(&(*(*currentHalfEdge->getNext()).getVertex()));
            }
            else
            {
                returnedVerticesIndices.push_back((currentHalfEdge->getVertexIndex()));
                //returnedVertices.push_back(&(*currentHalfEdge->getVertex()));
            }

            if (currentHalfEdge->getTwinIndex() == HalfEdgeDS::MAX_INDEX_VAL)
            {
                if (previouslyVisitedBoundaryEdge)
                {
                    visitedTwoBoundaryEdges = true;
                }
                else
                {
                    previouslyVisitedBoundaryEdge = true;
                    currentHalfEdge = &(*startingHalfEdge->getPrevious());
                    //musim taktiez nastavit ze som ziskal tento currentHalfEdge
                    //pomocou Previousa
                    previouslyAccessedNext = false;

                    returnedVerticesIndices.push_back((currentHalfEdge->getVertexIndex()));
                    //returnedVertices.push_back(&(*currentHalfEdge->getVertex()));

                    if (currentHalfEdge->getTwinIndex() != HalfEdgeDS::MAX_INDEX_VAL)
                    {
                        //ak je v opacnom smere
                        if ((*currentHalfEdge->getVertex()) != (*currentHalfEdge->getTwin()->getVertex()))
                        {
                            currentHalfEdge = &(*currentHalfEdge->getTwin()->getPrevious());
                            //musim taktiez nastavit ze som ziskal tento currentHalfEdge
                            //pomocou Previousa
                            previouslyAccessedNext = false;

                        }
                        else //ak je v rovnakom smere
                        {
                            currentHalfEdge = &(*currentHalfEdge->getTwin()->getNext());
                            //musim taktiez nastavit ze som ziskal tento currentHalfEdge
                            //pomocou Nexta
                            previouslyAccessedNext = true;
                        }
                    }
                    else
                    {
                        visitedTwoBoundaryEdges = true;
                        //skoncil algoritmus
                    }
                }
            }
            else //ak existuje twin
            {
                if (previouslyAccessedNext)
                {

                    //ak je v opacnom smere
                    if ((*currentHalfEdge->getVertex()) != (*currentHalfEdge->getTwin()->getVertex()))
                    {
                        currentHalfEdge = &(*currentHalfEdge->getTwin()->getNext());
                        //musim taktiez nastavit ze som ziskal tento currentHalfEdge
                        //pomocou Nexta
                        previouslyAccessedNext = true;

                    }
                    else //ak je v rovnakom smere
                    {
                        currentHalfEdge = &(*currentHalfEdge->getTwin()->getPrevious());
                        //musim taktiez nastavit ze som ziskal tento currentHalfEdge
                        //pomocou Previousa
                        previouslyAccessedNext = false;
                    }
                }
                else
                {

                    //ak je v opacnom smere
                    if ((*currentHalfEdge->getVertex()) != (*currentHalfEdge->getTwin()->getVertex()))
                    {
                        currentHalfEdge = &(*currentHalfEdge->getTwin()->getPrevious());
                        //musim taktiez nastavit ze som ziskal tento currentHalfEdge
                        //pomocou Previousa
                        previouslyAccessedNext = false;

                    }
                    else //ak je v rovnakom smere
                    {
                        currentHalfEdge = &(*currentHalfEdge->getTwin()->getNext());
                        //musim taktiez nastavit ze som ziskal tento currentHalfEdge
                        //pomocou Nexta
                        previouslyAccessedNext = true;
                    }

                }
            }

        } while ((currentHalfEdge != startingHalfEdge) && !visitedTwoBoundaryEdges);

        return std::make_pair(returnedVerticesIndices, returnedFacesIndices);
    }
    
};
