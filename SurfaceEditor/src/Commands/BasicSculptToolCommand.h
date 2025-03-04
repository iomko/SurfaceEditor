#pragma once
#include <iostream>
#include <glm/vec3.hpp>

#include "../Patterns/command.h"
#include "../ViewPortsHolder.h"
#include "../Callbacks/SelectFaceCallBack.h"

class BasicSculptToolCommand : public Command
{
public:

    virtual void execute() override
    {

    }

    glm::vec3 computeAverageNormal(const std::vector<glm::vec3>& normals) {
        glm::vec3 sum(0.0f);
        float size = normals.size();
        for (const auto& normal : normals) {
            sum += normal;
        }

        glm::vec3 averageNormal = sum / size;

        return glm::normalize(averageNormal);
    }

    std::pair<bool, float> isPointInsideSphere(const glm::vec3& point, const glm::vec3& sphereCenter, float sphereRadius) {
        float distance = glm::distance(point, sphereCenter);
        bool isInside = (distance <= sphereRadius);
        return std::make_pair(isInside, distance);
    }

    float logarithmicInterpolation(float edge0, float edge1, float x) {
        // Scale, bias and saturate x to 0..1 range
        x = std::max(0.0f, std::min((x - edge0) / (edge1 - edge0), 1.0f));
        return log(x * (exp(1) - 1) + 1) / log(exp(1));
    }

    float smoothstep(float edge0, float edge1, float x) {
        // Scale, bias and saturate x to 0..1 range
        x = std::max(0.0f, std::min((x - edge0) / (edge1 - edge0), 1.0f));
        return x * x * (3 - 2 * x);
        //return x * x * x * (x * (x * 6 - 15) + 10);
    }

    float calculateBrushScalingFactor(float distance, float radius, float brushStrength) {
        float t = 1.0f - (distance / radius);
        t = std::clamp(t, 0.0f, 1.0f);

        float scalingFactor = smoothstep(0.0f, 1.0f, t);
        scalingFactor *= brushStrength;

        return scalingFactor;
    }

    std::pair<std::vector<HalfEdgeDS::VertexIndex>, std::vector<HalfEdgeDS::FaceIndex>> accessNeighbouringVertsAndFacesOfVertex(HalfEdgeDS::Vertex* vertex)
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

    /*
    glm::vec3 calculate3DNormalOfHalfEdgeTriangleFace(HalfEdgeDS::Face* face)
    {
        HalfEdgeDS::HalfEdge firstHalfEdge = *face->getHalfEdge();

        HalfEdgeDS::Vertex* v1 = &*firstHalfEdge.getVertex();
        HalfEdgeDS::Vertex* v2 = &*firstHalfEdge.getNext()->getVertex();
        HalfEdgeDS::Vertex* v3 = &*firstHalfEdge.getNext()->getNext()->getVertex();

        // Calculate two vectors representing two edges of the triangle
        glm::vec3 edge1 = glm::vec3(v2->getPosition() - v1->getPosition());
        glm::vec3 edge2 = glm::vec3(v3->getPosition() - v1->getPosition());

        // Calculate the normal vector using cross product
        glm::vec3 normal = glm::cross(edge1, edge2);

        // Normalize the normal vector
        return glm::normalize(normal);
    }
    */

    void applyBrush(Ray& ray, float brushRadius, float brushStrength)
    {
        SelectFaceParamsIn selectFaceParamsIn;
        selectFaceParamsIn.ray = ray;
        SelectFaceParamsOut selectFaceParamsOut;

        SelectFaceCallBack selectFaceCallBack;
        selectFaceCallBack.execute(selectFaceParamsIn, selectFaceParamsOut);

        glm::vec3 retHitPoint = selectFaceParamsOut.hitPoint;
        HalfEdgeDS::Face* retFace = selectFaceParamsOut.face;
        Mesh* retMesh = selectFaceParamsOut.mesh;


        if (retFace != nullptr)
        {
            //tak vieme ze sme hitli face
            Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;

            const auto& selectedFace = retFace;
            Mesh* selectedMesh = retMesh;

            //---SCENERENDERERDATA---
			SceneRendererData::MeshFacesMap& meshFaceMap = scene->m_rendererData.meshData.meshFacesMap;
			SceneRendererData::MeshFacesMap::iterator meshFacesMapIt = meshFaceMap.find(selectedMesh);
            SceneRendererData::FaceInfoMap& faceInfoMap = meshFacesMapIt->second;

            //VAO
            SceneRendererData::MeshVaoDataMap& meshVaoDataMap = scene->m_rendererData.meshData.meshVaoDataMap;
            SceneRendererData::MeshVaoDataMap::iterator meshVaoDataMapIt = meshVaoDataMap.find(selectedMesh);
            SceneRendererData::MaterialVertexMap& materialVertexMap = meshVaoDataMapIt->second;
            //---SCENERENDERERDATA---


            //musime ist cez vsetky vertices danej faci
            auto faceVertexBegin = selectedFace->faceVertexBegin();
            auto faceVertexEnd = selectedFace->faceVertexEnd();

            float minVertexDistance = std::numeric_limits<float>::max();

            HalfEdgeDS::Vertex* closestVertex = nullptr;
            for (auto faceVertexIt = faceVertexBegin; faceVertexIt != faceVertexEnd; ++faceVertexIt)
            {
                const auto& currentVertexDistance = glm::distance(faceVertexIt.operator*().getPosition(), retHitPoint);
                if (currentVertexDistance < minVertexDistance)
                {
                    minVertexDistance = currentVertexDistance;
                    closestVertex = &faceVertexIt.operator*();
                }
            }
            //nasli sme najblizsi bod a teraz si musime vypocitat normalu ktoru budeme pouzivat, musime tak prejst cez vsetky faces daneho vertexu


            std::pair<std::vector<HalfEdgeDS::VertexIndex>, std::vector<HalfEdgeDS::FaceIndex>> accessedNeighboringFacesAndVerts = accessNeighbouringVertsAndFacesOfVertex(closestVertex);

            //pridaj si normaly jednotlivych facov do vektora
            std::vector<glm::vec3> neighbouringFacesNormals;
            for (const auto& accessedFace : accessedNeighboringFacesAndVerts.second)
            {
                //HalfEdgeDS::FaceIndex faceIndex = accessedFace;
                //size_t faceIndex = std::distance(selectedMesh->getHalfEdgeStructure()->faceIterBegin(), accessedFace->getHalfEdge()->getFace());

                //tuto na to aby sme mohli accessnut FaceInfo, tak potrebujeme mat k dispozicii Mesh
                //nasledne mat k dispozicii aj Face*
                HalfEdgeDS::Face* face = &selectedMesh->m_halfEdgeStructure->m_faces.at(accessedFace);

                SceneRendererData::FaceInfoMap::iterator faceInfoMapIt = faceInfoMap.find(face->getHalfEdge().operator*().getFaceIndex());
                SceneRendererData::FaceInfo faceInfo = faceInfoMapIt->second;

                neighbouringFacesNormals.push_back(faceInfo.normal);

                //neighbouringFacesNormals.push_back(selectedMesh->m_facesData.at(faceIndex).normal);
            }

            //musime vypocitat ich average normalu
            const auto& calculatedAverageNormal = computeAverageNormal(neighbouringFacesNormals);




            //teraz si zoberem poziciu vektora closesVertex
            //a na nom vytvorim virtualnu gulu, pricom jej polomer je specifikovany a taktiez aj sila brushe
            const auto& sphereCenter = retHitPoint;
            const auto& sphereRadius = brushRadius;




            std::unordered_set<HalfEdgeDS::Vertex*> uniqueSetOfVertices;
            std::vector<HalfEdgeDS::Vertex*> verticesInInsertionOrder;
            int currentAdditionVertexIndex = 0;
            //takze zo zaciatku tam pridam closestVertex


            std::pair<bool, float> intersectionAmountPair = isPointInsideSphere(closestVertex->getPosition(), sphereCenter, sphereRadius);
            if (intersectionAmountPair.first)
            {
                if (uniqueSetOfVertices.insert(closestVertex).second) {
                    verticesInInsertionOrder.push_back(closestVertex);
                }
            }

            //predpokladajme ze currentAdditionVertexIndex je 0
            //predpokladajme ze verticesInInsertionOrder.size() je 1

            //musim si taktiez vytvorit std::set facov ktore sa nakoniec z octree vymazu a nanovo sa tam pridaju

            std::set<HalfEdgeDS::Face*> octreeDeletedFaces;


            while ((currentAdditionVertexIndex <= (verticesInInsertionOrder.size() - 1)) && (!verticesInInsertionOrder.empty()))
            {
                //to znamena ze musim s nou pohnut
                intersectionAmountPair = isPointInsideSphere(verticesInInsertionOrder.at(currentAdditionVertexIndex)->getPosition(), sphereCenter, sphereRadius);


                const auto& scalingFactor = calculateBrushScalingFactor(intersectionAmountPair.second, sphereRadius, brushStrength);


                glm::vec3 vertexPositionBeforeChange = verticesInInsertionOrder.at(currentAdditionVertexIndex)->getPosition();
                verticesInInsertionOrder.at(currentAdditionVertexIndex)->setPosition(verticesInInsertionOrder.at(currentAdditionVertexIndex)->getPosition() + (calculatedAverageNormal * scalingFactor));


                //iba raz tu
                //pridaj ich do vektora

                accessedNeighboringFacesAndVerts = accessNeighbouringVertsAndFacesOfVertex(verticesInInsertionOrder.at(currentAdditionVertexIndex));

                for (auto& accessedVertex : accessedNeighboringFacesAndVerts.first)
                {

                    intersectionAmountPair = isPointInsideSphere(selectedMesh->getHalfEdgeStructure()->m_vertices.at(accessedVertex).getPosition(), sphereCenter, sphereRadius);
                    if (intersectionAmountPair.first)
                    {
                        if (uniqueSetOfVertices.insert(&selectedMesh->getHalfEdgeStructure()->m_vertices.at(accessedVertex)).second) {
                            verticesInInsertionOrder.push_back(&selectedMesh->getHalfEdgeStructure()->m_vertices.at(accessedVertex));
                        }
                    }
                }

                //musim aktualizovat edges

                //to znamena ze musim prejst cez vsetky faces daneho vertexu
                //budem chodit

                /*
                auto meshesShaderIt = ViewPortsHolderContext::m_viewPortsHolder->m_meshesShaderData.find(selectedMesh);
                if (meshesShaderIt != ViewPortsHolderContext::m_viewPortsHolder->m_meshesShaderData.end()) {
                    ViewPortHolder::MeshRenderingVAOData& vaoData = std::get<2>(meshesShaderIt->second);
                    vaoData.m_points.at(verticesInInsertionOrder.at(currentAdditionVertexIndex)->getHalfEdge()->getVertexIndex()).position = verticesInInsertionOrder.at(currentAdditionVertexIndex)->getPosition();
                }
                */

                //selectedMesh->m_meshRenderingData.m_points.at(verticesInInsertionOrder.at(currentAdditionVertexIndex)->getHalfEdge()->getVertexIndex()).position = verticesInInsertionOrder.at(currentAdditionVertexIndex)->getPosition();

                for (const auto& accessedFace : accessedNeighboringFacesAndVerts.second)
                {
                    octreeDeletedFaces.insert(&selectedMesh->getHalfEdgeStructure()->m_faces.at(accessedFace));

                    //musim aktualizovat aj edges a zatial to urobim jednoducho a to takym sposobom ze prejdem cez vsetky edges danej face a updatnim ich
                    //avsak chcelo by to prechadzat len cez tie ktore su susedmi s prave vybranym vertexom

                    auto faceHalfEdgeBegin = selectedMesh->getHalfEdgeStructure()->m_faces.at(accessedFace).faceHalfEdgeBegin();
                    auto faceHalfEdgeEnd = selectedMesh->getHalfEdgeStructure()->m_faces.at(accessedFace).faceHalfEdgeEnd();



                    //musime prechadzat cez vsetky halfEdges danej faci
                    for (auto faceHalfEdgeIt = faceHalfEdgeBegin; faceHalfEdgeIt != faceHalfEdgeEnd; ++faceHalfEdgeIt) {
                        // Access the vertex using the iterator


                        auto currentEdgeIndex = faceHalfEdgeIt.operator*().getEdgeIndex();


                        const auto& currentEdgeStartingVertex = faceHalfEdgeIt.operator*().getEdge()->getFirstVertex();
                        const auto& currentEdgeEndingVertex = faceHalfEdgeIt.operator*().getEdge()->getSecondVertex();


                        /*
                        auto meshesShaderIt = ViewPortsHolderContext::m_viewPortsHolder->m_meshesShaderData.find(selectedMesh);
                        if (meshesShaderIt != ViewPortsHolderContext::m_viewPortsHolder->m_meshesShaderData.end()) {
                            ViewPortHolder::MeshRenderingVAOData& vaoData = std::get<2>(meshesShaderIt->second);
                            vaoData.m_edges.at(currentEdgeIndex * 2).point = currentEdgeStartingVertex->getPosition() + (calculatedAverageNormal * 0.005f);
                            vaoData.m_edges.at((currentEdgeIndex * 2) + 1).point = currentEdgeEndingVertex->getPosition() + (calculatedAverageNormal * 0.005f);
                        }
                        */

                        //selectedMesh->m_meshRenderingData.m_edges.at(currentEdgeIndex * 2).point = currentEdgeStartingVertex->getPosition() + (calculatedAverageNormal * 0.005f);
                        //selectedMesh->m_meshRenderingData.m_edges.at((currentEdgeIndex * 2) + 1).point = currentEdgeEndingVertex->getPosition() + (calculatedAverageNormal * 0.005f);
                    }





                    //cheme updatnut vertices
                    //faceVertexBegin = selectedMesh->getHalfEdgeStructure()->m_faces.at(accessedFace).faceVertexBegin();
                    //faceVertexEnd = selectedMesh->getHalfEdgeStructure()->m_faces.at(accessedFace).faceVertexEnd();

                    //zbytovne ideme cez vsetky vertices


                    //prechadzame nasledne cez vsetky faces daneho vertexu;

                    //musim teraz prechadzat cez dane faces a najst vertex ktora by sa rovnala vertexPositionBeforeChange
                    //musim si najst zaciatocny IndexFaci a konecny IndexFaci

                    
                    //tuto musim proste len ziskat faceInfo pre dany accessedFace, to je vsetko
                    HalfEdgeDS::Face* face = &selectedMesh->m_halfEdgeStructure->m_faces.at(accessedFace);
					SceneRendererData::FaceInfoMap::iterator faceInfoMapIt = faceInfoMap.find(face->getHalfEdge().operator*().getFaceIndex());
					SceneRendererData::FaceInfo& faceInfo = faceInfoMapIt->second;
                    
                    
                    //new calculated faceNormal
                    glm::vec3 newCalculatedFaceNormal = PolygonOperations::computeFaceNormal(face);
                    //updatnut tuto normalu jednak pre VAO vektory ale aj pre facesData v Meshi
                    faceInfo.normal = newCalculatedFaceNormal;

					const auto& faceStartIndex = faceInfo.startIndex;
					const auto& faceEndIndex = faceInfo.endIndex;
                    for (int currentFaceIndex = faceStartIndex; currentFaceIndex <= faceEndIndex; ++currentFaceIndex)
                    {

                        //const auto& material = selectedMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(currentFaceIndex).material;
                        //const auto& indexInMaterial = selectedMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(currentFaceIndex).index;

                        //updatnut faceNormal aj vo VAO datach
                        SceneRendererData::MaterialVertexMap::iterator materialVertexMapIt = materialVertexMap.find(faceInfo.material);
                        std::vector<MeshVertex>& materialVertexVector = materialVertexMapIt->second;

                        materialVertexVector.at(currentFaceIndex).normal = newCalculatedFaceNormal;


                        if(materialVertexVector.at(currentFaceIndex).position == vertexPositionBeforeChange)
                        {
                            materialVertexVector.at(currentFaceIndex).position = verticesInInsertionOrder.at(currentAdditionVertexIndex)->getPosition();
                        }

                        //a teraz musim aj aktualizovat jednotlive positions


                        //aj normalu pre combined vector
                        //selectedMesh->m_combinedVertexDataMatVector.m_vertexData.at(currentFaceIndex).normal = newCalculatedFaceNormal;
                        //normal taktiez pre material vector
                        //selectedMesh->m_materialIDVertexDataMap.at(material).m_vertexData.at(indexInMaterial).normal = newCalculatedFaceNormal;

                        /*
                        if (selectedMesh->m_combinedVertexDataMatVector.m_vertexData.at(currentFaceIndex).position == vertexPositionBeforeChange)
                        {
                            selectedMesh->m_combinedVertexDataMatVector.m_vertexData.at(currentFaceIndex).position = verticesInInsertionOrder.at(currentAdditionVertexIndex)->getPosition();
                            selectedMesh->m_materialIDVertexDataMap.at(material).m_vertexData.at(indexInMaterial).position = verticesInInsertionOrder.at(currentAdditionVertexIndex)->getPosition();

                        }
                        */
                    }

                }

                //pridat dalsie
                ++currentAdditionVertexIndex;

            }

            //vymazanie z octree

            //std::map <Mesh*, std::map<HalfEdgeDS::Face*, std::vector<glm::vec3>>> m_meshesFaceOctreeMap;
            //std::map <glm::vec3, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>> m_meshFaceOctreesMap;

             //std::map <Mesh*, std::map<HalfEdgeDS::Face*, std::vector<glm::vec3>>> m_meshesFaceOctreeMap;

            auto meshFacesOctreeIt = ViewPortsHolderContext::m_viewPortsHolder->m_scene->meshFaceOctreeCoordsMap.find(selectedMesh);
            for (const auto& octreeDeletedFace : octreeDeletedFaces)
            {

                auto faceOctreesIt = meshFacesOctreeIt->second.find(octreeDeletedFace);

                if (faceOctreesIt != meshFacesOctreeIt->second.end())
                {
                    for (const auto& octreeId : faceOctreesIt->second) {

                        auto octreeIdOctreeIt = ViewPortsHolderContext::m_viewPortsHolder->m_scene->coordsOctreeMap.find(octreeId);

                        if (octreeIdOctreeIt != ViewPortsHolderContext::m_viewPortsHolder->m_scene->coordsOctreeMap.end())
                        {
                            octreeIdOctreeIt->second.removeData(std::make_pair(selectedMesh, octreeDeletedFace));
                        }
                        //po tom co som vymazal tuto facu, sa musim taktiez pozriet ci je dany octree prazdny, lebo ak je tak ho vymazem aj z tej mapy
                        if (octreeIdOctreeIt->second.rootNode == nullptr)
                        {
                            std::cout << "NULLPTR" << std::endl;
                        }
                        if (octreeIdOctreeIt->second.rootNode->dataCount == 0)
                        {
                            ViewPortsHolderContext::m_viewPortsHolder->m_scene->coordsOctreeMap.erase(octreeId);
                        }
                    }


                    //vymazem zaznam z prvej std::mapy
                    meshFacesOctreeIt->second.erase(octreeDeletedFace);



                    //std::map <Mesh*, std::map<HalfEdgeDS::Face*, std::vector<glm::vec3>>> m_meshesFaceOctreeMap;
                    //std::map <glm::vec3, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>> m_meshFaceOctreesMap;




                    //pridanie naspat




                    Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;



                    //potrebujem ziskat boundind box daneho facu

                    

					std::vector<HalfEdgeDS::Vertex> faceVerts;
                    selectedMesh->m_halfEdgeStructure->getVerticesFromFace(octreeDeletedFace->getHalfEdge()->getFace(), faceVerts);
                    //auto& faceVerts = selectedMesh->m_halfEdgeStructure->getVerticesFromFace(octreeDeletedFace->getHalfEdge()->getFace());
                    AABBBoundingRegion faceBounds(
                        faceVerts.begin(),
                        faceVerts.end(),
                        [](HalfEdgeDS::Vertex& p) { return p.getPosition().x; },
                        [](HalfEdgeDS::Vertex& p) { return p.getPosition().y; },
                        [](HalfEdgeDS::Vertex& p) { return p.getPosition().z; }
                    );

                    //musime teraz vytvorit vsetky octrees alebo ak existuju octrees tak tam pridat tuto facu.

                    //calculate the X,Y,Z for MinBound


                    //glm::vec3 voxelIndexMinBound = SceneUtilities::getVoxelIndex(scene->voxelSize, faceBounds.getMin());
                    glm::vec3 voxelIndexMinBound = SceneUtilities::getVoxelIndex(faceBounds.getMin(), scene);
                    glm::vec3 voxelIndexMaxBound = SceneUtilities::getVoxelIndex(faceBounds.getMax(), scene);



                    auto insertedFaceOctreeIdsMap = meshFacesOctreeIt->second.insert(std::make_pair(octreeDeletedFace, std::vector<glm::vec3>{}));


                    for (int x = voxelIndexMinBound.x; x <= voxelIndexMaxBound.x; ++x)
                    {
                        for (int y = voxelIndexMinBound.y; y <= voxelIndexMaxBound.y; ++y)
                        {
                            for (int z = voxelIndexMinBound.z; z <= voxelIndexMaxBound.z; ++z)
                            {
                                glm::vec3 currentIndexBound = { x,y,z };
                                //teraz sme ziskali IndexBound pre facu. Teraz sa musime pozriet ci uz existuje octree s tymto indexom
                                auto it = scene->coordsOctreeMap.find(currentIndexBound);
                                if (it != scene->coordsOctreeMap.end())
                                {
                                    //existuje octree s tymto indexom
                                    it->second.addDataToOctree(std::make_pair(selectedMesh, &(*octreeDeletedFace)), faceBounds);

                                }
                                else
                                {
                                    //SceneUtilities::calculateOctreeBounds()
                                    //auto [octreeMinBound, octreeMaxBound] = scene->calculateOctreeBounds(currentIndexBound, scene->voxelXSize);
                                    auto [octreeMinBound, octreeMaxBound] = SceneUtilities::calculateOctreeBounds(currentIndexBound, scene);
                                    auto addedOctree = scene->coordsOctreeMap.emplace(currentIndexBound, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>(octreeMinBound, octreeMaxBound)).first;
                                    //neexistuje octree s tymto indexom
                                    addedOctree->second.addDataToOctree(std::make_pair(selectedMesh, &(*octreeDeletedFace)), faceBounds);

                                }

                                insertedFaceOctreeIdsMap.first->second.push_back(currentIndexBound);

                            }
                        }
                    }

                    //vymazat std::vector octrees

                    //najskor v druhej mape


                    //to ale znamena vymazat tento zaznam aj v druhej std::map

                }


                //teraz som vymazal danu facu v daneho octree
                //teraz ju potrebujem znovu pridat

                //avsak

            }

        }
    }

    void undo()
    {

    }

    static constexpr std::string_view getCommandName() noexcept { return "BasicSculptToolCommand"; }
};