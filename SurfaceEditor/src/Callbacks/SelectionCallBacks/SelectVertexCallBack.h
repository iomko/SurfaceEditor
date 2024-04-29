#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include "../Callback.h"
#include "../../ViewPortHolder.h"
#include "../../AABBBoundingRegion.h"

class SelectVertexCallBack : public Callback
{
public:
    SelectVertexCallBack(ViewPortHolder* viewPortHolder)
    {
        m_viewPortHolder = viewPortHolder;
    }

    void execute() override
    {
        std::cout << "SELECTVERTEXCALLBACK" << std::endl;


        //select Mesh
        Scene* scene = m_viewPortHolder->m_viewPortLayer->getScene();

        Ray ray = m_viewPortHolder->m_viewPortLayer->getCamera()->m_cameraRay;


        const auto& octreeAlg = [](const std::pair<Mesh*, HalfEdgeDS::Face*>& faceMeshPair, const Ray& ray)->std::pair<bool, float>
            {
                size_t faceIndex = std::distance(faceMeshPair.first->getMeshData()->m_faces.begin(), faceMeshPair.second->getHalfEdge()->getFace());
                size_t faceStartIndex = faceMeshPair.first->m_facesData.at(faceIndex).startIndex;
                size_t faceEndIndex = faceMeshPair.first->m_facesData.at(faceIndex).endIndex;
                for (int i = faceStartIndex; i < faceEndIndex; i += 3)
                {
                    //musime vytvorit trojuholniky
                    const auto& firstVertex = faceMeshPair.first->m_combinedVertexDataMatVector.m_vertexData.at(i).position;
                    const auto& secondVertex = faceMeshPair.first->m_combinedVertexDataMatVector.m_vertexData.at(i + 1).position;
                    const auto& thirdVertex = faceMeshPair.first->m_combinedVertexDataMatVector.m_vertexData.at(i + 2).position;

                    float amountToBeMultiplied;
                    if (AABBBoundingRegion::rayTriangleIntersect(ray.origin, ray.direction, firstVertex, secondVertex, thirdVertex, amountToBeMultiplied))
                    {
                        return std::make_pair(true, amountToBeMultiplied);
                    }

                }
                return std::make_pair(false, -1.0f);
            };

        float minOctreeHitDistance = std::numeric_limits<float>::max();
        HalfEdgeDS::Face* retFace = nullptr;
        Mesh* retMesh = nullptr;
        for (auto& meshFaceOctree : scene->m_meshFaceOctreesMap)
        {
            const auto& octreeRetData = meshFaceOctree.second.findDataInOctree(ray, octreeAlg);

            const auto& octreeNode = std::get<0>(octreeRetData);
            const auto& mesh = std::get<1>(octreeRetData).first;
            const auto& face = std::get<1>(octreeRetData).second;
            const auto& distance = std::get<2>(octreeRetData);

            if (octreeNode != nullptr)
            {
                if (distance < minOctreeHitDistance)
                {
                    retMesh = mesh;
                    retFace = face;
                    minOctreeHitDistance = distance;
                }
            }
        }








        glm::vec3 hitPointOfFace = ray.origin + (ray.direction * minOctreeHitDistance);

        if(retMesh != nullptr)
        {
	        //tak vieme ze sme klikni na jednu z facov

            auto selectedMeshesIter = m_viewPortHolder->m_selectedMeshes.find(retMesh);

            if (selectedMeshesIter != m_viewPortHolder->m_selectedMeshes.end())
            {
                
                std::vector<std::pair<HalfEdgeDS::Vertex*, float>> vertexDistancePairs;

                for (auto faceIt = retFace->faceVertexBegin(); faceIt != retFace->faceVertexEnd(); ++faceIt) {
                    HalfEdgeDS::Vertex* currentVertexInFace = &(*faceIt);
                    std::pair<bool, float> pointInSpherePair = isPointInsideSphere(hitPointOfFace, currentVertexInFace->getPosition(), 5.0f);

                    if(pointInSpherePair.first)
                    {
                        vertexDistancePairs.push_back(std::make_pair(currentVertexInFace, pointInSpherePair.second));
                    }
                }

                //vyberem najblizsi vertex
                float minVertexDistance = std::numeric_limits<float>::max();
                HalfEdgeDS::Vertex* closestVertex = nullptr;

                for (const auto& pair : vertexDistancePairs) {
                    float distance = pair.second;
                    if (distance < minVertexDistance) {
                        minVertexDistance = distance;
                        closestVertex = pair.first;
                    }
                }

                //musime sa pozriet ci sme vobec nasli closestVertex
                if(closestVertex != nullptr)
                {
                    //ked uz mam najblizsi vertex tak ho pridam do selectedVertices

                //predtym sa vsak musim pozriet ci tento Mesh* entry uz existuje.

                    auto selectedVerticesIt = m_viewPortHolder->m_selectedVertices.find(retMesh);

                    if (selectedVerticesIt == m_viewPortHolder->m_selectedVertices.end()) {
                        // If the Mesh* entry doesn't exist, insert a new entry with an empty set
                        std::set<HalfEdgeDS::Vertex*> vertexSet;
                        vertexSet.insert(closestVertex);

                        //rendering point

                        auto meshesShaderIt = m_viewPortHolder->m_meshesShaderData.find(retMesh);
                        if (meshesShaderIt != m_viewPortHolder->m_meshesShaderData.end()) {
                            ViewPortHolder::MeshRenderingVAOData& vaoData = std::get<2>(meshesShaderIt->second);
                            vaoData.m_points.at(closestVertex->getHalfEdge()->getVertexIndex()).isHighlighted = 1.0f;
                        }


                        //retMesh->m_meshRenderingData.m_points.at(closestVertex->getHalfEdge()->getVertexIndex()).isHighlighted = 1.0f;
                        // Insert the new entry with the Mesh* and the new set into the map
                        m_viewPortHolder->m_selectedVertices.emplace(retMesh, vertexSet);
                    }
                    else
                    {
                        selectedVerticesIt->second.insert(closestVertex);

                        auto meshesShaderIt = m_viewPortHolder->m_meshesShaderData.find(retMesh);
                        if (meshesShaderIt != m_viewPortHolder->m_meshesShaderData.end()) {
                            ViewPortHolder::MeshRenderingVAOData& vaoData = std::get<2>(meshesShaderIt->second);
                            vaoData.m_points.at(closestVertex->getHalfEdge()->getVertexIndex()).isHighlighted = 1.0f;
                        }


                        //rendering point
                        //retMesh->m_meshRenderingData.m_points.at(closestVertex->getHalfEdge()->getVertexIndex()).isHighlighted = 1.0f;
                    }

                    //teraz som uz pridal dany vertex do std::mapy
                    //avsak po tom co som ho pridal si musim prejst vsetkymi jeho neighboring facami
                    //accessni vsetky neighboring faces daneho vertexu

                    std::pair<std::vector<HalfEdgeDS::VertexIndex>, std::vector<HalfEdgeDS::FaceIndex>> neighboringFacesVerticesIndices = accessNeighbouringVertsAndFacesOfSelectedVertex(closestVertex);

                    //teraz musim prejst cez vsetky faces

                    //zoberem si iterator pre selectedVertices a selectedFaces daneho meshu
                    auto selectedVerticesMap = m_viewPortHolder->m_selectedVertices.find(retMesh);
                    auto selectedFacesMap = m_viewPortHolder->m_selectedFaces.find(retMesh);

                    for (const auto& faceIndex : neighboringFacesVerticesIndices.second)
                    {
                        HalfEdgeDS::Face* face = &retMesh->getMeshData()->m_faces.at(faceIndex);

                        //teraz musim prechadzat vsetky vertices daneho facu

                        bool faceSelected = true;
                        for (auto faceIt = face->faceVertexBegin(); faceIt != face->faceVertexEnd(); ++faceIt) {
                            HalfEdgeDS::Vertex* currentVertexInFace = &(*faceIt);

                            //a teraz sa musim pozriet ci vsetky vertices daneho facu su uz obsiahnute v selectedVertices

                            //to znamena ze akonahle sa aspon jeden nenachadza tak tento face mozem skipnut a prejst na dalsi v poradi

                            // Check if the Face* is inside the set
                            std::set<HalfEdgeDS::Vertex*>& vertexSet = selectedVerticesMap->second;
                            auto vertexSetIt = vertexSet.find(currentVertexInFace);
                            if (vertexSetIt == vertexSet.end()) {
                                //vieme ze obsiahnuty neni
                                faceSelected = false;
                                break;
                            }

                        }
                        if (faceSelected)
                        {
                            //v takom pripade musim pridat tento face do selectedFaces taktiez

                            //avsak predtym sa musime pozriet ci existuje selectedFacesMap

                            if(selectedFacesMap == m_viewPortHolder->m_selectedFaces.end())
                            {
	                            //to znamena ze este nemame ziadny zaznam o selectedFaces

                                // Insert the new Face* into a new std::set
                                std::set<HalfEdgeDS::Face*> faceSet;
                                faceSet.insert(face);

                                // Insert the new Mesh* and associated std::set into the map
                                m_viewPortHolder->m_selectedFaces.emplace(retMesh, faceSet); // Inserting the std::set directly


                                //taktiez upravim renderingData
                                HalfEdgeDS::FaceIndex faceIndex = face->getHalfEdge()->getFaceIndex();
                                size_t faceStartIndex = retMesh->m_facesData.at(faceIndex).startIndex;
                                size_t faceEndIndex = retMesh->m_facesData.at(faceIndex).endIndex;

                                //ist do m_combinedVectora
                                for (int i = faceStartIndex; i <= faceEndIndex; ++i)
                                {
                                    //m_combinedVector
                                    retMesh->m_combinedVertexDataMatVector.m_vertexData.at(i).isHighlited = 1.0f;
                                    //material vectors
                                    Material* materialInMaterialVector = retMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).material;
                                    size_t indexInMaterialVector = retMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).index;
                                    auto materialVectorIt = retMesh->m_materialIDVertexDataMap.find(materialInMaterialVector);
                                    materialVectorIt->second.m_vertexData.at(indexInMaterialVector).isHighlited = 1.0f;
                                }


                            } else
                            {
	                            //uz mame vytvoreny zaznam o selectedFaces
                                std::set<HalfEdgeDS::Face*>& faceSet = selectedFacesMap->second;
                                faceSet.insert(face);


                                //taktiez upravim renderingData
                                HalfEdgeDS::FaceIndex faceIndex = face->getHalfEdge()->getFaceIndex();
                                size_t faceStartIndex = retMesh->m_facesData.at(faceIndex).startIndex;
                                size_t faceEndIndex = retMesh->m_facesData.at(faceIndex).endIndex;

                                //ist do m_combinedVectora
                                for (int i = faceStartIndex; i <= faceEndIndex; ++i)
                                {
                                    //m_combinedVector
                                    retMesh->m_combinedVertexDataMatVector.m_vertexData.at(i).isHighlited = 1.0f;
                                    //material vectors
                                    Material* materialInMaterialVector = retMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).material;
                                    size_t indexInMaterialVector = retMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).index;
                                    auto materialVectorIt = retMesh->m_materialIDVertexDataMap.find(materialInMaterialVector);
                                    materialVectorIt->second.m_vertexData.at(indexInMaterialVector).isHighlited = 1.0f;
                                }


                            }
                        }

                    }
                } else
                {
	                //nenasiel sa najblizsi vertex!
                }
            }
            else
            {
                //tak vymazem vsetky selectedFaces
                //tak vymazen vsetky selectedVertices
                for (const auto& selectedMesh : m_viewPortHolder->m_selectedMeshes)
                {
                    auto selectedFacesOfSelectedMeshIter = m_viewPortHolder->m_selectedFaces.find(selectedMesh);
                    auto selectedVerticesOfSelectedMeshIter = m_viewPortHolder->m_selectedVertices.find(selectedMesh);


                    for (HalfEdgeDS::Face* currentFace : selectedFacesOfSelectedMeshIter->second) {

                        //taktiez upravim renderingData
                        HalfEdgeDS::FaceIndex faceIndex = currentFace->getHalfEdge()->getFaceIndex();
                        size_t faceStartIndex = selectedMesh->m_facesData.at(faceIndex).startIndex;
                        size_t faceEndIndex = selectedMesh->m_facesData.at(faceIndex).endIndex;

                        //ist do m_combinedVectora
                        for (int i = faceStartIndex; i <= faceEndIndex; ++i)
                        {
                            //m_combinedVector
                            selectedMesh->m_combinedVertexDataMatVector.m_vertexData.at(i).isHighlited = 0.0f;
                            //material vectors
                            Material* materialInMaterialVector = selectedMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).material;
                            size_t indexInMaterialVector = selectedMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).index;
                            auto materialVectorIt = selectedMesh->m_materialIDVertexDataMap.find(materialInMaterialVector);
                            materialVectorIt->second.m_vertexData.at(indexInMaterialVector).isHighlited = 0.0f;
                        }


                    }


                    if (selectedFacesOfSelectedMeshIter != m_viewPortHolder->m_selectedFaces.end()) {
                        m_viewPortHolder->m_selectedFaces.erase(selectedFacesOfSelectedMeshIter);
                    }


                    //taktiez vymazem vsetky vertices
                    if (selectedVerticesOfSelectedMeshIter != m_viewPortHolder->m_selectedVertices.end()) {

                        //taktiez upravim renderingData

                        for (HalfEdgeDS::Vertex* currentVertex : selectedVerticesOfSelectedMeshIter->second) {

                            //taktiez upravim renderingData
                            HalfEdgeDS::VertexIndex vertexIndex = currentVertex->getHalfEdge()->getVertexIndex();

                            auto meshesShaderIt = m_viewPortHolder->m_meshesShaderData.find(retMesh);
                            if (meshesShaderIt != m_viewPortHolder->m_meshesShaderData.end()) {
                                ViewPortHolder::MeshRenderingVAOData& vaoData = std::get<2>(meshesShaderIt->second);
                                vaoData.m_points.at(vertexIndex).isHighlighted = 0.0f;
                            }

                            //selectedMesh->m_meshRenderingData.m_points.at(vertexIndex).isHighlighted = 0.0f;

                        }

                        m_viewPortHolder->m_selectedVertices.erase(selectedVerticesOfSelectedMeshIter);
                    }


                }
            }

        }
        else
        {
            //vtedy vymazem vsetky selectedFaces daneho meshu
            //vtedy vymazem vsetky selectedVertices daneho meshu

            //tak prejdem cez vsetky selectedMeshes a vymazem dane selectedFaces
            //tak prejdem cez vsetky selectedMeshes a vymazem dane selectedVertices

            for (const auto& selectedMesh : m_viewPortHolder->m_selectedMeshes)
            {
                auto selectedFacesOfSelectedMeshIter = m_viewPortHolder->m_selectedFaces.find(selectedMesh);
                auto selectedVerticesOfSelectedMeshIter = m_viewPortHolder->m_selectedVertices.find(selectedMesh);
                

                if(selectedFacesOfSelectedMeshIter != m_viewPortHolder->m_selectedFaces.end())
                {

                    for (HalfEdgeDS::Face* currentFace : selectedFacesOfSelectedMeshIter->second) {

                        //taktiez upravim renderingData
                        HalfEdgeDS::FaceIndex faceIndex = currentFace->getHalfEdge()->getFaceIndex();
                        size_t faceStartIndex = selectedMesh->m_facesData.at(faceIndex).startIndex;
                        size_t faceEndIndex = selectedMesh->m_facesData.at(faceIndex).endIndex;

                        //ist do m_combinedVectora
                        for (int i = faceStartIndex; i <= faceEndIndex; ++i)
                        {
                            //m_combinedVector
                            selectedMesh->m_combinedVertexDataMatVector.m_vertexData.at(i).isHighlited = 0.0f;
                            //material vectors
                            Material* materialInMaterialVector = selectedMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).material;
                            size_t indexInMaterialVector = selectedMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(i).index;
                            auto materialVectorIt = selectedMesh->m_materialIDVertexDataMap.find(materialInMaterialVector);
                            materialVectorIt->second.m_vertexData.at(indexInMaterialVector).isHighlited = 0.0f;
                        }


                    }

                    m_viewPortHolder->m_selectedFaces.erase(selectedFacesOfSelectedMeshIter);

                }


                //taktiez vymazem vsetky vertices
                if (selectedVerticesOfSelectedMeshIter != m_viewPortHolder->m_selectedVertices.end()) {

                    //taktiez upravim renderingData

                    for (HalfEdgeDS::Vertex* currentVertex : selectedVerticesOfSelectedMeshIter->second) {

                        //taktiez upravim renderingData
                        HalfEdgeDS::VertexIndex vertexIndex = currentVertex->getHalfEdge()->getVertexIndex();


                        auto meshesShaderIt = m_viewPortHolder->m_meshesShaderData.find(retMesh);
                        if (meshesShaderIt != m_viewPortHolder->m_meshesShaderData.end()) {
                            ViewPortHolder::MeshRenderingVAOData& vaoData = std::get<2>(meshesShaderIt->second);
                            vaoData.m_points.at(vertexIndex).isHighlighted = 0.0f;
                        }

                        //selectedMesh->m_meshRenderingData.m_points.at(vertexIndex).isHighlighted = 0.0f;

                    }

                    m_viewPortHolder->m_selectedVertices.erase(selectedVerticesOfSelectedMeshIter);
                }

            }

        }


        //temporary
        //prejdi cez vsetky selektnute meshe a faces a vypis ich


        for (const auto& entry : m_viewPortHolder->m_selectedFaces) {
            std::cout << "SelectedMesh ID: " << entry.first->m_meshID << std::endl;
            for (HalfEdgeDS::Face* face : entry.second) {
                std::cout << "---SelectedFace ID: " << face->getHalfEdge()->getFaceIndex() << std::endl;
            }
        }

        for (const auto& entry : m_viewPortHolder->m_selectedVertices) {
            for (HalfEdgeDS::Vertex* vertex : entry.second) {
                std::cout << "---SelectedVertex ID: " << vertex->getHalfEdge()->getVertexIndex() << std::endl;
            }
        }

        std::cout << "-------------------------------------------------------" << std::endl;
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

    ViewPortHolder* m_viewPortHolder;
};
