#pragma once
#include <iostream>

#include "FacesDataTransfer.h"
#include "../Callback.h"
#include "../../ViewPortHolder.h"
#include "../../AABBBoundingRegion.h"

class SelectFaceCallBack : public Callback
{
public:
    SelectFaceCallBack(ViewPortHolder* viewPortHolder)
    {
        m_viewPortHolder = viewPortHolder;
    }

    void execute() override
    {
        std::cout << "SELECTFACECALLBACK" << std::endl;

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

        if (retMesh != nullptr)
        {
            //avsak mesh tej faci musi existovat v m_selectedMeshes, inac nemozem tuto facu pridat

            //tu sa pozrem ci ten retMesh sa nachadza v selectedMeshes

            auto selectedMeshesIter = m_viewPortHolder->m_selectedMeshes.find(retMesh);

            if(selectedMeshesIter != m_viewPortHolder->m_selectedMeshes.end())
            {
	            //tak sa este pozriem ci existuje v mojej m_selectedFaces

                auto selectedFacesIter = m_viewPortHolder->m_selectedFaces.find(retMesh);

                if(selectedFacesIter == m_viewPortHolder->m_selectedFaces.end())
                {
	                //tak vytvorim novy set a pridam do selectedFaces
					std::set<HalfEdgeDS::Face*> facesSet;
					facesSet.insert(retFace);
					m_viewPortHolder->m_selectedFaces.insert(std::make_pair(retMesh, facesSet));

                    auto selectedVerticesIter = m_viewPortHolder->m_selectedVertices.find(retMesh);

                    if(selectedVerticesIter == m_viewPortHolder->m_selectedVertices.end())
                    {
	                    //tak vtedy tam ten zaznam vytvorim

                        std::set<HalfEdgeDS::Vertex*> newSelectedVertices;

                        for (auto it = retFace->faceVertexBegin(); it != retFace->faceVertexEnd(); ++it) {
                            HalfEdgeDS::Vertex* vertex = &(*it);
                            newSelectedVertices.insert(vertex);
                            //taktiez upravim renderingData

                            auto meshesShaderIt = m_viewPortHolder->m_meshesShaderData.find(retMesh);
                            if (meshesShaderIt != m_viewPortHolder->m_meshesShaderData.end()) {
                                ViewPortHolder::MeshRenderingVAOData& vaoData = std::get<2>(meshesShaderIt->second);
                                vaoData.m_points.at(vertex->getHalfEdge()->getVertexIndex()).isHighlighted = 1.0f;
                            }

                            //retMesh->m_meshRenderingData.m_points.at(vertex->getHalfEdge()->getVertexIndex()).isHighlighted = 1.0f;
                        }
                        m_viewPortHolder->m_selectedVertices.emplace(retMesh, newSelectedVertices);

                        //akurat ze teraz este musim prejst cez vsetky vertices prave pridaneho facu
                        //a poznamenat si, ktore faces su temporarne (pridat ked sa prepneme do vertex modu)
                        for (auto& selectedVertex : newSelectedVertices)
                        {
                            //musime najst vsetky faces daneho vertexu
                            std::pair<std::vector<HalfEdgeDS::VertexIndex>, std::vector<HalfEdgeDS::FaceIndex>> neighboringFacesVerticesIndices = accessNeighbouringVertsAndFacesOfSelectedVertex(selectedVertex);


                            selectedFacesIter = m_viewPortHolder->m_selectedFaces.find(retMesh);
                            selectedVerticesIter = m_viewPortHolder->m_selectedVertices.find(retMesh);

                            for (const auto& faceIndex : neighboringFacesVerticesIndices.second)
                            {
                                HalfEdgeDS::Face* face = &retMesh->getMeshData()->m_faces.at(faceIndex);

                                //teraz musim prechadzat vsetky vertices daneho facu
                                //predtym sa ale pozriem ci tato face uz je v selected
                                if (selectedFacesIter->second.find(face) == selectedFacesIter->second.end())
                                {
                                    //ma zmysel skumat ci bude selektnuta alebo nie
                                    bool faceSelected = true;
                                    for (auto faceIt = face->faceVertexBegin(); faceIt != face->faceVertexEnd(); ++faceIt) {
                                        HalfEdgeDS::Vertex* currentVertexInFace = &(*faceIt);

                                        //a teraz sa musim pozriet ci vsetky vertices daneho facu su uz obsiahnute v selectedVertices

                                        //to znamena ze akonahle sa aspon jeden nenachadza tak tento face mozem skipnut a prejst na dalsi v poradi

                                        // Check if the Face* is inside the set

                                        std::set<HalfEdgeDS::Vertex*>& vertexSet = selectedVerticesIter->second;
                                        auto vertexSetIt = vertexSet.find(currentVertexInFace);
                                        if (vertexSetIt == vertexSet.end()) {
                                            //vieme ze obsiahnuty neni
                                            faceSelected = false;
                                            break;
                                        }
                                    }
                                    if (faceSelected)
                                    {
                                        auto it = FacesDataTransfer::m_transferedData.find(retMesh);

                                        if (it != FacesDataTransfer::m_transferedData.end()) {
                                            std::set<HalfEdgeDS::Face*>& transferedFacesData = it->second;
                                            transferedFacesData.insert(face);
                                        }
                                        else {
                                            std::set<HalfEdgeDS::Face*> newTransferedFacesSet;
                                            newTransferedFacesSet.insert(face);
                                            FacesDataTransfer::m_transferedData.emplace(retMesh, newTransferedFacesSet);
                                        }

                                    }
                                }

                            }
                        }
                        

                    } else
                    {
	                    //vtedy do uz existujuceho zaznamu pridam vertices
                        for (auto it = retFace->faceVertexBegin(); it != retFace->faceVertexEnd(); ++it) {
                            HalfEdgeDS::Vertex* vertex = &(*it);
                            selectedVerticesIter->second.insert(vertex);
                            //taktiez upravim renderingData

                            auto meshesShaderIt = m_viewPortHolder->m_meshesShaderData.find(retMesh);
                            if (meshesShaderIt != m_viewPortHolder->m_meshesShaderData.end()) {
                                ViewPortHolder::MeshRenderingVAOData& vaoData = std::get<2>(meshesShaderIt->second);
                                vaoData.m_points.at(vertex->getHalfEdge()->getVertexIndex()).isHighlighted = 1.0f;
                            }

                            //retMesh->m_meshRenderingData.m_points.at(vertex->getHalfEdge()->getVertexIndex()).isHighlighted = 1.0f;


                            //musime najst vsetky faces daneho vertexu
                            std::pair<std::vector<HalfEdgeDS::VertexIndex>, std::vector<HalfEdgeDS::FaceIndex>> neighboringFacesVerticesIndices = accessNeighbouringVertsAndFacesOfSelectedVertex(vertex);


                            selectedFacesIter = m_viewPortHolder->m_selectedFaces.find(retMesh);
                            selectedVerticesIter = m_viewPortHolder->m_selectedVertices.find(retMesh);

                            for (const auto& faceIndex : neighboringFacesVerticesIndices.second)
                            {
                                HalfEdgeDS::Face* face = &retMesh->getMeshData()->m_faces.at(faceIndex);

                                //teraz musim prechadzat vsetky vertices daneho facu
                                //predtym sa ale pozriem ci tato face uz je v selected
                                if (selectedFacesIter->second.find(face) == selectedFacesIter->second.end())
                                {
                                    //ma zmysel skumat ci bude selektnuta alebo nie
                                    bool faceSelected = true;
                                    for (auto faceIt = face->faceVertexBegin(); faceIt != face->faceVertexEnd(); ++faceIt) {
                                        HalfEdgeDS::Vertex* currentVertexInFace = &(*faceIt);

                                        //a teraz sa musim pozriet ci vsetky vertices daneho facu su uz obsiahnute v selectedVertices

                                        //to znamena ze akonahle sa aspon jeden nenachadza tak tento face mozem skipnut a prejst na dalsi v poradi

                                        // Check if the Face* is inside the set

                                        std::set<HalfEdgeDS::Vertex*>& vertexSet = selectedVerticesIter->second;
                                        auto vertexSetIt = vertexSet.find(currentVertexInFace);
                                        if (vertexSetIt == vertexSet.end()) {
                                            //vieme ze obsiahnuty neni
                                            faceSelected = false;
                                            break;
                                        }
                                    }
                                    if (faceSelected)
                                    {
                                        auto it = FacesDataTransfer::m_transferedData.find(retMesh);

                                        if (it != FacesDataTransfer::m_transferedData.end()) {
                                            std::set<HalfEdgeDS::Face*>& transferedFacesData = it->second;
                                            transferedFacesData.insert(face);
                                        }
                                        else {
                                            std::set<HalfEdgeDS::Face*> newTransferedFacesSet;
                                            newTransferedFacesSet.insert(face);
                                            FacesDataTransfer::m_transferedData.emplace(retMesh, newTransferedFacesSet);
                                        }
                                    }
                                }

                            }



                        }
                    }


                    //taktiez upravim renderingData
                    HalfEdgeDS::FaceIndex faceIndex = retFace->getHalfEdge()->getFaceIndex();
                    size_t faceStartIndex = retMesh->m_facesData.at(faceIndex).startIndex;
                    size_t faceEndIndex = retMesh->m_facesData.at(faceIndex).endIndex;

                    //ist do m_combinedVectora
                    for(int i = faceStartIndex; i <= faceEndIndex; ++i)
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
	                //tak pridam do selectedFaces
					selectedFacesIter->second.insert(retFace);


                    auto selectedVerticesIter = m_viewPortHolder->m_selectedVertices.find(retMesh);

                    if (selectedVerticesIter == m_viewPortHolder->m_selectedVertices.end())
                    {
                        //tak vtedy tam ten zaznam vytvorim
                        std::set<HalfEdgeDS::Vertex*> newSelectedVertices;

                        for (auto it = retFace->faceVertexBegin(); it != retFace->faceVertexEnd(); ++it) {
                            HalfEdgeDS::Vertex* vertex = &(*it);
                            newSelectedVertices.insert(vertex);
                            //taktiez upravim renderingData

                            auto meshesShaderIt = m_viewPortHolder->m_meshesShaderData.find(retMesh);
                            if (meshesShaderIt != m_viewPortHolder->m_meshesShaderData.end()) {
                                ViewPortHolder::MeshRenderingVAOData& vaoData = std::get<2>(meshesShaderIt->second);
                                vaoData.m_points.at(vertex->getHalfEdge()->getVertexIndex()).isHighlighted = 1.0f;
                            }

                            //retMesh->m_meshRenderingData.m_points.at(vertex->getHalfEdge()->getVertexIndex()).isHighlighted = 1.0f;



                            //musime najst vsetky faces daneho vertexu
                            std::pair<std::vector<HalfEdgeDS::VertexIndex>, std::vector<HalfEdgeDS::FaceIndex>> neighboringFacesVerticesIndices = accessNeighbouringVertsAndFacesOfSelectedVertex(vertex);


                            selectedFacesIter = m_viewPortHolder->m_selectedFaces.find(retMesh);
                            selectedVerticesIter = m_viewPortHolder->m_selectedVertices.find(retMesh);

                            for (const auto& faceIndex : neighboringFacesVerticesIndices.second)
                            {
                                HalfEdgeDS::Face* face = &retMesh->getMeshData()->m_faces.at(faceIndex);

                                //teraz musim prechadzat vsetky vertices daneho facu
                                //predtym sa ale pozriem ci tato face uz je v selected
                                if (selectedFacesIter->second.find(face) == selectedFacesIter->second.end())
                                {
                                    //ma zmysel skumat ci bude selektnuta alebo nie
                                    bool faceSelected = true;
                                    for (auto faceIt = face->faceVertexBegin(); faceIt != face->faceVertexEnd(); ++faceIt) {
                                        HalfEdgeDS::Vertex* currentVertexInFace = &(*faceIt);

                                        //a teraz sa musim pozriet ci vsetky vertices daneho facu su uz obsiahnute v selectedVertices

                                        //to znamena ze akonahle sa aspon jeden nenachadza tak tento face mozem skipnut a prejst na dalsi v poradi

                                        // Check if the Face* is inside the set

                                        std::set<HalfEdgeDS::Vertex*>& vertexSet = selectedVerticesIter->second;
                                        auto vertexSetIt = vertexSet.find(currentVertexInFace);
                                        if (vertexSetIt == vertexSet.end()) {
                                            //vieme ze obsiahnuty neni
                                            faceSelected = false;
                                            break;
                                        }
                                    }
                                    if (faceSelected)
                                    {
                                        auto it = FacesDataTransfer::m_transferedData.find(retMesh);

                                        if (it != FacesDataTransfer::m_transferedData.end()) {
                                            std::set<HalfEdgeDS::Face*>& transferedFacesData = it->second;
                                            transferedFacesData.insert(face);
                                        }
                                        else {
                                            std::set<HalfEdgeDS::Face*> newTransferedFacesSet;
                                            newTransferedFacesSet.insert(face);
                                            FacesDataTransfer::m_transferedData.emplace(retMesh, newTransferedFacesSet);
                                        }
                                    }
                                }

                            }



                        }
                        m_viewPortHolder->m_selectedVertices.emplace(retMesh, newSelectedVertices);


                    }
                    else
                    {
                        //vtedy do uz existujuceho zaznamu pridam vertices
                        for (auto it = retFace->faceVertexBegin(); it != retFace->faceVertexEnd(); ++it) {
                            HalfEdgeDS::Vertex* vertex = &(*it);
                            selectedVerticesIter->second.insert(vertex);
                            //taktiez upravim renderingData

                            auto meshesShaderIt = m_viewPortHolder->m_meshesShaderData.find(retMesh);
                            if (meshesShaderIt != m_viewPortHolder->m_meshesShaderData.end()) {
                                ViewPortHolder::MeshRenderingVAOData& vaoData = std::get<2>(meshesShaderIt->second);
                                vaoData.m_points.at(vertex->getHalfEdge()->getVertexIndex()).isHighlighted = 1.0f;
                            }

                            //retMesh->m_meshRenderingData.m_points.at(vertex->getHalfEdge()->getVertexIndex()).isHighlighted = 1.0f;


                            //musime najst vsetky faces daneho vertexu
                            std::pair<std::vector<HalfEdgeDS::VertexIndex>, std::vector<HalfEdgeDS::FaceIndex>> neighboringFacesVerticesIndices = accessNeighbouringVertsAndFacesOfSelectedVertex(vertex);


                            selectedFacesIter = m_viewPortHolder->m_selectedFaces.find(retMesh);
                            selectedVerticesIter = m_viewPortHolder->m_selectedVertices.find(retMesh);

                            for (const auto& faceIndex : neighboringFacesVerticesIndices.second)
                            {
                                HalfEdgeDS::Face* face = &retMesh->getMeshData()->m_faces.at(faceIndex);

                                //teraz musim prechadzat vsetky vertices daneho facu
                                //predtym sa ale pozriem ci tato face uz je v selected
                                if (selectedFacesIter->second.find(face) == selectedFacesIter->second.end())
                                {
                                    //ma zmysel skumat ci bude selektnuta alebo nie
                                    bool faceSelected = true;
                                    for (auto faceIt = face->faceVertexBegin(); faceIt != face->faceVertexEnd(); ++faceIt) {
                                        HalfEdgeDS::Vertex* currentVertexInFace = &(*faceIt);

                                        //a teraz sa musim pozriet ci vsetky vertices daneho facu su uz obsiahnute v selectedVertices

                                        //to znamena ze akonahle sa aspon jeden nenachadza tak tento face mozem skipnut a prejst na dalsi v poradi

                                        // Check if the Face* is inside the set

                                        std::set<HalfEdgeDS::Vertex*>& vertexSet = selectedVerticesIter->second;
                                        auto vertexSetIt = vertexSet.find(currentVertexInFace);
                                        if (vertexSetIt == vertexSet.end()) {
                                            //vieme ze obsiahnuty neni
                                            faceSelected = false;
                                            break;
                                        }
                                    }
                                    if (faceSelected)
                                    {
                                        auto it = FacesDataTransfer::m_transferedData.find(retMesh);

                                        if (it != FacesDataTransfer::m_transferedData.end()) {
                                            std::set<HalfEdgeDS::Face*>& transferedFacesData = it->second;
                                            transferedFacesData.insert(face);
                                        }
                                        else {
                                            std::set<HalfEdgeDS::Face*> newTransferedFacesSet;
                                            newTransferedFacesSet.insert(face);
                                            FacesDataTransfer::m_transferedData.emplace(retMesh, newTransferedFacesSet);
                                        }
                                    }
                                }

                            }


                        }
                    }


                    //taktiez upravim renderingData
                    HalfEdgeDS::FaceIndex faceIndex = retFace->getHalfEdge()->getFaceIndex();
                    size_t faceStartIndex = retMesh->m_facesData.at(faceIndex).startIndex;
                    size_t faceEndIndex = retMesh->m_facesData.at(faceIndex).endIndex;

                    //ist do m_combinedVectora
                    for (size_t i = faceStartIndex; i <= faceEndIndex; ++i)
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

            } else
            {
	            //tak vymazeem vsetky selectedFaces
                for (const auto& selectedMesh : m_viewPortHolder->m_selectedMeshes)
                {
                    auto selectedFacesOfSelectedMeshIter = m_viewPortHolder->m_selectedFaces.find(selectedMesh);
                    auto selectedVerticesOfSelectedMeshIter = m_viewPortHolder->m_selectedVertices.find(selectedMesh);

                    if (selectedFacesOfSelectedMeshIter != m_viewPortHolder->m_selectedFaces.end()) {


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

                FacesDataTransfer::m_transferedData.clear();

            }

        }
        else
        {
            //vtedy vymazem vsetky selectedFaces daneho meshu

            //tak prejdem cez vsetky selectedMeshes a vymazem dane selectedFaces

            for(const auto& selectedMesh : m_viewPortHolder->m_selectedMeshes)
            {

                auto selectedFacesOfSelectedMeshIter = m_viewPortHolder->m_selectedFaces.find(selectedMesh);
                auto selectedVerticesOfSelectedMeshIter = m_viewPortHolder->m_selectedVertices.find(selectedMesh);

                if (selectedFacesOfSelectedMeshIter != m_viewPortHolder->m_selectedFaces.end()) {


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

            FacesDataTransfer::m_transferedData.clear();

        }


        //temporary
        //prejdi cez vsetky selektnute meshe a faces a vypis ich


        for (const auto& entry : m_viewPortHolder->m_selectedFaces) {
            std::cout << "SelectedMesh ID: " << entry.first->m_meshID << std::endl;
            for (HalfEdgeDS::Face* face : entry.second) {
                std::cout << "---SelectedFace ID: " << face->getHalfEdge()->getFaceIndex() << std::endl;
            }
        }

        std::cout << "-------------------------------------------------------" << std::endl;
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


private:
    ViewPortHolder* m_viewPortHolder;
};
