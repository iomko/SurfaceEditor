#pragma once
#include <iostream>
#include <glm/vec3.hpp>

#include "../Patterns/command.h"
#include "../ViewPortHolder.h"

class BasicSculptToolCommand : public Command
{
public:
	BasicSculptToolCommand(ViewPortHolder* viewPortHolder)
	{
		m_viewPortHolder = viewPortHolder;
	}

	void execute()
	{

	}


    glm::vec3 computeAverageNormal(const std::vector<glm::vec3>& normals) {
        glm::vec3 sum(0.0f);
        float size = normals.size();  // Casting to float here
        for (const auto& normal : normals) {
            sum += normal;
        }

        // Compute the average
        glm::vec3 averageNormal = sum / size;

        return glm::normalize(averageNormal);
    }

    std::pair<bool, float> isPointInsideSphere(const glm::vec3& point, const glm::vec3& sphereCenter, float sphereRadius) {
        float distance = glm::distance(point, sphereCenter);
        bool isInside = (distance <= sphereRadius);
        return std::make_pair(isInside, distance);
    }

    /*
    float calculateBrushScalingFactor(float distance, float radius, float brushStrength) {
        float scalingFactor = 1.0f / (1.0f + distance / radius);
        scalingFactor *= brushStrength;
        return scalingFactor;
    }
    */

    float logarithmicInterpolation(float edge0, float edge1, float x) {
        // Scale, bias and saturate x to 0..1 range
        x = std::max(0.0f, std::min((x - edge0) / (edge1 - edge0), 1.0f));
        // Evaluate logarithmic function
        return log(x * (exp(1) - 1) + 1) / log(exp(1));
    }

    float smoothstep(float edge0, float edge1, float x) {
        // Scale, bias and saturate x to 0..1 range
        x = std::max(0.0f, std::min((x - edge0) / (edge1 - edge0), 1.0f));
        // Evaluate polynomial
        return x * x * (3 - 2 * x);
        //return x * x * x * (x * (x * 6 - 15) + 10);
    }


    float calculateBrushScalingFactor(float distance, float radius, float brushStrength) {
        float t = 1.0f - (distance / radius); // Map distance to [0, 1] range
        t = std::clamp(t, 0.0f, 1.0f); // Ensure t stays within [0, 1]

        float scalingFactor = smoothstep(0.0f, 1.0f, t); // Use smoothstep for interpolation
        scalingFactor *= brushStrength;
        
        return scalingFactor;
    }

    const std::pair<std::pair<Mesh*, HalfEdgeDS::Face*>, glm::vec3>& selectFace(const Ray& ray)
    {
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
        
        for (auto& meshFaceOctree : m_viewPortHolder->m_viewPortLayer->getScene()->m_meshFaceOctreesMap)
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
        return std::make_pair(std::make_pair(retMesh, retFace), ray.origin + (glm::normalize(ray.direction) * minOctreeHitDistance));
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
    
    void applyBrush(Ray& ray, float brushRadius, float brushStrength)
    {

        auto& selectedFaceHitPointPair = selectFace(ray);

        if (selectedFaceHitPointPair.first.second != nullptr)
        {
            //tak vieme ze sme hitli face


            const auto& selectedFace = selectedFaceHitPointPair.first.second;
            Mesh* selectedMesh = selectedFaceHitPointPair.first.first;

            //musime ist cez vsetky vertices danej faci
            auto faceVertexBegin = selectedFace->faceVertexBegin();
            auto faceVertexEnd = selectedFace->faceVertexEnd();

            float minVertexDistance = std::numeric_limits<float>::max();

            HalfEdgeDS::Vertex* closestVertex = nullptr;
            for (auto faceVertexIt = faceVertexBegin; faceVertexIt != faceVertexEnd; ++faceVertexIt)
            {
                const auto& currentVertexDistance = glm::distance(faceVertexIt.operator*().getPosition(), selectedFaceHitPointPair.second);
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
                HalfEdgeDS::FaceIndex faceIndex = accessedFace;
                //size_t faceIndex = std::distance(selectedMesh->getMeshData()->faceIterBegin(), accessedFace->getHalfEdge()->getFace());
                neighbouringFacesNormals.push_back(selectedMesh->m_facesData.at(faceIndex).normal);
            }

            //musime vypocitat ich average normalu
            const auto& calculatedAverageNormal = computeAverageNormal(neighbouringFacesNormals);




            //teraz si zoberem poziciu vektora closesVertex
            //a na nom vytvorim virtualnu gulu, pricom jej polomer je specifikovany a taktiez aj sila brushe
            const auto& sphereCenter = selectedFaceHitPointPair.second;
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

            
            while((currentAdditionVertexIndex <= (verticesInInsertionOrder.size() - 1)) && (!verticesInInsertionOrder.empty()))
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
                    
                    intersectionAmountPair = isPointInsideSphere(selectedMesh->getMeshData()->m_vertices.at(accessedVertex).getPosition(), sphereCenter, sphereRadius);
                    if (intersectionAmountPair.first)
                    {
                        if (uniqueSetOfVertices.insert(&selectedMesh->getMeshData()->m_vertices.at(accessedVertex)).second) {
                            verticesInInsertionOrder.push_back(&selectedMesh->getMeshData()->m_vertices.at(accessedVertex));
                        }
                    }
                }

                //musim aktualizovat edges

                //to znamena ze musim prejst cez vsetky faces daneho vertexu
                //budem chodit


                auto meshesShaderIt = m_viewPortHolder->m_meshesShaderData.find(selectedMesh);
                if (meshesShaderIt != m_viewPortHolder->m_meshesShaderData.end()) {
                    ViewPortHolder::MeshRenderingVAOData& vaoData = std::get<2>(meshesShaderIt->second);
                    vaoData.m_points.at(verticesInInsertionOrder.at(currentAdditionVertexIndex)->getHalfEdge()->getVertexIndex()).position = verticesInInsertionOrder.at(currentAdditionVertexIndex)->getPosition();
                }


                //selectedMesh->m_meshRenderingData.m_points.at(verticesInInsertionOrder.at(currentAdditionVertexIndex)->getHalfEdge()->getVertexIndex()).position = verticesInInsertionOrder.at(currentAdditionVertexIndex)->getPosition();

                for (const auto& accessedFace : accessedNeighboringFacesAndVerts.second)
                {
                    octreeDeletedFaces.insert(&selectedMesh->getMeshData()->m_faces.at(accessedFace));

                    //musim aktualizovat aj edges a zatial to urobim jednoducho a to takym sposobom ze prejdem cez vsetky edges danej face a updatnim ich
                    //avsak chcelo by to prechadzat len cez tie ktore su susedmi s prave vybranym vertexom

                    auto faceHalfEdgeBegin = selectedMesh->getMeshData()->m_faces.at(accessedFace).faceHalfEdgeBegin();
                    auto faceHalfEdgeEnd = selectedMesh->getMeshData()->m_faces.at(accessedFace).faceHalfEdgeEnd();



                    //musime prechadzat cez vsetky halfEdges danej faci
                    for (auto faceHalfEdgeIt = faceHalfEdgeBegin; faceHalfEdgeIt != faceHalfEdgeEnd; ++faceHalfEdgeIt) {
                        // Access the vertex using the iterator
                        

                        auto currentEdgeIndex = faceHalfEdgeIt.operator*().getEdgeIndex();


                        const auto& currentEdgeStartingVertex = faceHalfEdgeIt.operator*().getEdge()->getFirstVertex();
                        const auto& currentEdgeEndingVertex = faceHalfEdgeIt.operator*().getEdge()->getSecondVertex();


                        auto meshesShaderIt = m_viewPortHolder->m_meshesShaderData.find(selectedMesh);
                        if (meshesShaderIt != m_viewPortHolder->m_meshesShaderData.end()) {
                            ViewPortHolder::MeshRenderingVAOData& vaoData = std::get<2>(meshesShaderIt->second);
                            vaoData.m_edges.at(currentEdgeIndex * 2).point = currentEdgeStartingVertex->getPosition() + (calculatedAverageNormal * 0.005f);
                            vaoData.m_edges.at((currentEdgeIndex * 2) + 1).point = currentEdgeEndingVertex->getPosition() + (calculatedAverageNormal * 0.005f);
                        }


                        //selectedMesh->m_meshRenderingData.m_edges.at(currentEdgeIndex * 2).point = currentEdgeStartingVertex->getPosition() + (calculatedAverageNormal * 0.005f);
                        //selectedMesh->m_meshRenderingData.m_edges.at((currentEdgeIndex * 2) + 1).point = currentEdgeEndingVertex->getPosition() + (calculatedAverageNormal * 0.005f);
                    }





                    //cheme updatnut vertices
                    //faceVertexBegin = selectedMesh->getMeshData()->m_faces.at(accessedFace).faceVertexBegin();
                    //faceVertexEnd = selectedMesh->getMeshData()->m_faces.at(accessedFace).faceVertexEnd();

                    //zbytovne ideme cez vsetky vertices


                    //prechadzame nasledne cez vsetky faces daneho vertexu;

                    //musim teraz prechadzat cez dane faces a najst vertex ktora by sa rovnala vertexPositionBeforeChange
                    //musim si najst zaciatocny IndexFaci a konecny IndexFaci
                    const auto& faceStartIndex = selectedMesh->m_facesData.at(accessedFace).startIndex;
                    const auto& faceEndIndex = selectedMesh->m_facesData.at(accessedFace).endIndex;
                    //new calculated faceNormal
                    glm::vec3 newCalculatedFaceNormal = calculate3DNormalOfHalfEdgeTriangleFace(&selectedMesh->getMeshData()->m_faces.at(accessedFace));
                    //updatnut tuto normalu jednak pre VAO vektory ale aj pre facesData v Meshi
                    selectedMesh->m_facesData.at(accessedFace).normal = newCalculatedFaceNormal;



                    for (int currentFaceIndex = faceStartIndex; currentFaceIndex <= faceEndIndex; ++currentFaceIndex)
                    {
                        const auto& material = selectedMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(currentFaceIndex).material;
                        const auto& indexInMaterial = selectedMesh->m_combinedVertexDataMatVector.m_materialIDIndexMap.at(currentFaceIndex).index;



                        //aj normalu pre combined vector
                        selectedMesh->m_combinedVertexDataMatVector.m_vertexData.at(currentFaceIndex).normal = newCalculatedFaceNormal;
                        //normal taktiez pre material vector
                        selectedMesh->m_materialIDVertexDataMap.at(material).m_vertexData.at(indexInMaterial).normal = newCalculatedFaceNormal;


                        if (selectedMesh->m_combinedVertexDataMatVector.m_vertexData.at(currentFaceIndex).position == vertexPositionBeforeChange)
                        {
                            selectedMesh->m_combinedVertexDataMatVector.m_vertexData.at(currentFaceIndex).position = verticesInInsertionOrder.at(currentAdditionVertexIndex)->getPosition();
                            selectedMesh->m_materialIDVertexDataMap.at(material).m_vertexData.at(indexInMaterial).position = verticesInInsertionOrder.at(currentAdditionVertexIndex)->getPosition();

                        } 
                    }

                }
                
                //pridat dalsie
                ++currentAdditionVertexIndex;
                
            }

            //vymazanie z octree

            //std::map <Mesh*, std::map<HalfEdgeDS::Face*, std::vector<glm::vec3>>> m_meshesFaceOctreeMap;
            //std::map <glm::vec3, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>> m_meshFaceOctreesMap;

             //std::map <Mesh*, std::map<HalfEdgeDS::Face*, std::vector<glm::vec3>>> m_meshesFaceOctreeMap;
            auto meshFacesOctreeIt = m_viewPortHolder->m_viewPortLayer->getScene()->m_meshesFaceOctreeMap.find(selectedMesh);
            for (const auto& octreeDeletedFace : octreeDeletedFaces)
            {

                auto faceOctreesIt = meshFacesOctreeIt->second.find(octreeDeletedFace);
                
                if(faceOctreesIt != meshFacesOctreeIt->second.end())
                {
                    for (const auto& octreeId : faceOctreesIt->second) {

                        auto octreeIdOctreeIt = m_viewPortHolder->m_viewPortLayer->getScene()->m_meshFaceOctreesMap.find(octreeId);

                        if(octreeIdOctreeIt != m_viewPortHolder->m_viewPortLayer->getScene()->m_meshFaceOctreesMap.end())
                        {
                            octreeIdOctreeIt->second.removeData(std::make_pair(selectedMesh, octreeDeletedFace));
						}
                        //po tom co som vymazal tuto facu, sa musim taktiez pozriet ci je dany octree prazdny, lebo ak je tak ho vymazem aj z tej mapy
                        if(octreeIdOctreeIt->second.rootNode == nullptr)
                        {
                            std::cout << "NULLPTR" << std::endl;
                        }
                        if(octreeIdOctreeIt->second.rootNode->dataCount == 0)
                        {
                            m_viewPortHolder->m_viewPortLayer->getScene()->m_meshFaceOctreesMap.erase(octreeId); 
                        }
                    }


                    //vymazem zaznam z prvej std::mapy
                    meshFacesOctreeIt->second.erase(octreeDeletedFace);



                    //std::map <Mesh*, std::map<HalfEdgeDS::Face*, std::vector<glm::vec3>>> m_meshesFaceOctreeMap;
                	//std::map <glm::vec3, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>> m_meshFaceOctreesMap;

                    


                    //pridanie naspat




                    Scene* scene = m_viewPortHolder->m_viewPortLayer->getScene();



					//potrebujem ziskat boundind box daneho facu

                    auto& faceVerts = selectedMesh->m_halfEdgeMesh->getVerticesFromFace(octreeDeletedFace->getHalfEdge()->getFace());
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
                                auto it = scene->m_meshFaceOctreesMap.find(currentIndexBound);
                                if (it != scene->m_meshFaceOctreesMap.end())
                                {
                                    //existuje octree s tymto indexom
                                    it->second.addDataToOctree(std::make_pair(selectedMesh, &(*octreeDeletedFace)), faceBounds);

                                }
                                else
                                {
                                    //SceneUtilities::calculateOctreeBounds()
                                    //auto [octreeMinBound, octreeMaxBound] = scene->calculateOctreeBounds(currentIndexBound, scene->voxelXSize);
                                    auto [octreeMinBound, octreeMaxBound] = SceneUtilities::calculateOctreeBounds(currentIndexBound, scene);
                                    auto addedOctree = scene->m_meshFaceOctreesMap.emplace(currentIndexBound, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>(octreeMinBound, octreeMaxBound)).first;
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

private:
	ViewPortHolder* m_viewPortHolder;
};