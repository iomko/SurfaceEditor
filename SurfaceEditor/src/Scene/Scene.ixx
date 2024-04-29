module;
//#include "../DataStructures/HalfEdge.h"
#include "../Mesh.h"
#include "../AABBBoundingRegion.h"
#include "../Core/Input.h"
export module Scene;

import DataStructures;
import Camera;

export class Scene
{
public:

    //v octree budem tento mesh uchovavat ako std::pair<Mesh*, Face*>
    std::map <Mesh*, std::map<HalfEdgeDS::Face*, std::vector<glm::vec3>>> m_meshesFaceOctreeMap;
    std::map <glm::vec3, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>> m_meshFaceOctreesMap;
public:
    Scene(){}

    /*
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
    */


    /*
    glm::vec3 computeAverageNormal(const std::vector<glm::vec3>& normals) {
        glm::vec3 sum(0.0f);
        float size = normals.size();  // Casting to float here
        for (const auto& normal : normals) {
            sum += normal;
        }

        return sum / size;
    }

    std::pair<bool, float> isPointInsideSphere(const glm::vec3& point, const glm::vec3& sphereCenter, float sphereRadius) {
        float distance = glm::distance(point, sphereCenter);
        bool isInside = (distance <= sphereRadius);
        return std::make_pair(isInside, distance);
    }

    float calculateBrushScalingFactor(float distance, float radius, float brushStrength) {
        float scalingFactor = 1.0f / (1.0f + distance / radius);
        scalingFactor *= brushStrength;
        return scalingFactor;
    }
    

	
    void applyBrush(const Ray& ray, float brushRadius, float brushStrength)
    {
        
        auto& selectedFaceHitPointPair = selectFace(ray);

        if (selectedFaceHitPointPair.first.second != nullptr)
        {
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


            const auto& accessedNeighboringFacesAndVerts = accessNeighbouringVertsAndFacesOfVertex(closestVertex);

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
            const auto& sphereCenter = closestVertex->getPosition();
            const auto& sphereRadius = brushRadius;


            std::unordered_set<HalfEdgeDS::Vertex*> uniqueSetOfVertices;
            std::vector<HalfEdgeDS::Vertex*> verticesInInsertionOrder;
            int currentVertexIndex = 0;
            //takze zo zaciatku tam pridam closestVertex

            
            std::pair<bool, float> intersectionAmountPair = isPointInsideSphere(closestVertex->getPosition(), sphereCenter, sphereRadius);
            if (intersectionAmountPair.first)
            {
                if (uniqueSetOfVertices.insert(closestVertex).second) {
                    verticesInInsertionOrder.push_back(closestVertex);
                }
            }
            

            //je tam a potrebujem ten vertex spracovat
            const auto& scalingFactor = calculateBrushScalingFactor(intersectionAmountPair.second, sphereRadius, brushStrength);
            glm::vec3 vertexPositionBeforeChange = closestVertex->getPosition();
            closestVertex->setPosition(closestVertex->getPosition() + (calculatedAverageNormal * scalingFactor));
            //okrem toho musim aj samotne vertexData aktualizovat

            for (const auto& accessedFace : accessedNeighboringFacesAndVerts.second)
            {
                //musim teraz prechadzat cez dane faces a najst vertex ktora by sa rovnala vertexPositionBeforeChange
                std::cout << "FaceIndex: " << accessedFace << std::endl;
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

                    if(selectedMesh->m_combinedVertexDataMatVector.m_vertexData.at(currentFaceIndex).position == vertexPositionBeforeChange)
                    {
                        selectedMesh->m_combinedVertexDataMatVector.m_vertexData.at(currentFaceIndex).position = closestVertex->getPosition();
                        selectedMesh->m_materialIDVertexDataMap.at(material).m_vertexData.at(indexInMaterial).position = closestVertex->getPosition();

                    }
                }
            }



            while(true)
            {
	            //idem spracovat uz currentVertexIndex

                //znamena ze si musim zobrat jeho neighbouring faces a vertices

                //potrebujem vsak najskor spracovat seba
                const auto& accessedNeighboringFacesAndVerts = accessNeighbouringVertsAndFacesOfVertex(verticesInInsertionOrder.at(currentVertexIndex));


                for (const auto& accessedVertex : accessedNeighboringFacesAndVerts.first)
                {
                    intersectionAmountPair = isPointInsideSphere(closestVertex->getPosition(), sphereCenter, sphereRadius);
                    if (intersectionAmountPair.first)
                    {
                        if (uniqueSetOfVertices.insert(closestVertex).second) {
                            verticesInInsertionOrder.push_back(closestVertex);
                        }
                    }
                }
                ++currentVertexIndex;
                if(currentVertexIndex <= verticesInInsertionOrder.size()-1)
                {

                    //je tam a potrebujem ten vertex spracovat
                    const auto& scalingFactor = calculateBrushScalingFactor(glm::distance(verticesInInsertionOrder.at(currentVertexIndex)->getPosition(), sphereCenter), sphereRadius, brushStrength);
                    glm::vec3 vertexPositionBeforeChange = verticesInInsertionOrder.at(currentVertexIndex)->getPosition();
                    verticesInInsertionOrder.at(currentVertexIndex)->setPosition(closestVertex->getPosition() + (calculatedAverageNormal * scalingFactor));
                    //okrem toho musim aj samotne vertexData aktualizovat

                    for (const auto& accessedFace : accessedNeighboringFacesAndVerts.second)
                    {
                        //musim teraz prechadzat cez dane faces a najst vertex ktora by sa rovnala vertexPositionBeforeChange
                        std::cout << "FaceIndex: " << accessedFace << std::endl;
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
                                selectedMesh->m_combinedVertexDataMatVector.m_vertexData.at(currentFaceIndex).position = verticesInInsertionOrder.at(currentVertexIndex)->getPosition();
                                selectedMesh->m_materialIDVertexDataMap.at(material).m_vertexData.at(indexInMaterial).position = verticesInInsertionOrder.at(currentVertexIndex)->getPosition();

                            }
                        }
                    }

                }else
                {
                    break;
                }

            }
        }
    }
    */
    

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

    //zatial tieto metody su v scene
    const std::pair<std::pair<Mesh*,HalfEdgeDS::Face*>, glm::vec3>& selectFace(const Ray& ray)
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
        for (auto& meshFaceOctree : m_meshFaceOctreesMap)
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
        return std::make_pair(std::make_pair(retMesh,retFace), ray.origin + (glm::normalize(ray.direction) * minOctreeHitDistance));
	}
    

	
	Mesh* selectMesh(const Ray& ray)
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

        //musime prechadzat cez vsetky Octrees ktore mame v scene

        //inac by sme mohli prechadzat len cez octrees pre selektnute meshe

        
        float minOctreeHitDistance = std::numeric_limits<float>::max();
        Mesh* retMesh = nullptr;
        for (auto& meshFaceOctree : m_meshFaceOctreesMap)
        {
            const auto& octreeRetData = meshFaceOctree.second.findDataInOctree(ray, octreeAlg);

            const auto& octreeNode = std::get<0>(octreeRetData);
            const auto& mesh = std::get<1>(octreeRetData).first;
            const auto& distance = std::get<2>(octreeRetData);

            if(octreeNode != nullptr)
			{
				if(distance < minOctreeHitDistance)
				{
                    retMesh = mesh;
                    minOctreeHitDistance = distance;
				}
			}
        }
        return retMesh;
    }

    void addMesh2(Mesh* mesh)
    {
        // Check if there is already a record for the given mesh
        auto meshesFaceOctreeIt = m_meshesFaceOctreeMap.find(mesh);

        if (meshesFaceOctreeIt == m_meshesFaceOctreeMap.end()) {
            auto& faceOctreesMap = m_meshesFaceOctreeMap[mesh];


            for (auto faceIter = mesh->m_halfEdgeMesh->faceIterBegin(); faceIter != mesh->m_halfEdgeMesh->faceIterEnd(); ++faceIter)
            {
                auto& faceVerts = mesh->m_halfEdgeMesh->getVerticesFromFace(faceIter);
                AABBBoundingRegion faceBounds(
                    faceVerts.begin(),
                    faceVerts.end(),
                    [](HalfEdgeDS::Vertex& p) { return p.getPosition().x; },
                    [](HalfEdgeDS::Vertex& p) { return p.getPosition().y; },
                    [](HalfEdgeDS::Vertex& p) { return p.getPosition().z; }
                );

                //musime teraz vytvorit vsetky octrees alebo ak existuju octrees tak tam pridat tuto facu.

                //calculate the X,Y,Z for MinBound
                int voxelXIndexMinBound = getVoxelIndex(voxelXSize, faceBounds.getMin().x);
                int voxelYIndexMinBound = getVoxelIndex(voxelYSize, faceBounds.getMin().y);
                int voxelZIndexMinBound = getVoxelIndex(voxelZSize, faceBounds.getMin().z);

                int voxelXIndexMaxBound = getVoxelIndex(voxelXSize, faceBounds.getMax().x);
                int voxelYIndexMaxBound = getVoxelIndex(voxelYSize, faceBounds.getMax().y);
                int voxelZIndexMaxBound = getVoxelIndex(voxelZSize, faceBounds.getMax().z);



                for (int x = voxelXIndexMinBound; x <= voxelXIndexMaxBound; ++x)
                {
                    for (int y = voxelYIndexMinBound; y <= voxelYIndexMaxBound; ++y)
                    {
                        for (int z = voxelZIndexMinBound; z <= voxelZIndexMaxBound; ++z)
                        {
                            glm::vec3 currentIndexBound = { x,y,z };
                            //teraz sme ziskali IndexBound pre facu. Teraz sa musime pozriet ci uz existuje octree s tymto indexom
                            auto it = m_meshFaceOctreesMap.find(currentIndexBound);
                            if (it != m_meshFaceOctreesMap.end())
                            {
                                std::cout << "Existuje a bol pridany" << std::endl;
                                //existuje octree s tymto indexom
                                it->second.addDataToOctree(std::make_pair(mesh, &(*faceIter)),faceBounds);

                            }
                            else
                            {
                                auto [octreeMinBound, octreeMaxBound] = calculateOctreeBounds(currentIndexBound, voxelXSize);
                                auto addedOctree = m_meshFaceOctreesMap.emplace(currentIndexBound, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>(octreeMinBound, octreeMaxBound)).first;
                                //neexistuje octree s tymto indexom
                                std::cout << "Neexistuje a bol pridany" << std::endl;
                                addedOctree->second.addDataToOctree(std::make_pair(mesh, &(*faceIter)), faceBounds);

                            }

                            HalfEdgeDS::Face* facePointer = &(*faceIter);
                            glm::vec3 octreeIndex = currentIndexBound;
                            // Now add the entry to the inner map
                            faceOctreesMap[facePointer].push_back(octreeIndex);
                        }
                    }
                }


            }


        }
        
    }
    */

    ~Scene()
    {
    }
    
public:
    /*
    std::pair<glm::vec3, glm::vec3> calculateOctreeBounds(const glm::vec3& voxelIndex, float voxelSize) {
        glm::vec3 octreeMinBound(voxelIndex.x * voxelSize, voxelIndex.y * voxelSize, voxelIndex.z * voxelSize);
        glm::vec3 octreeMaxBound(octreeMinBound.x + voxelSize, octreeMinBound.y + voxelSize, octreeMinBound.z + voxelSize);
        return { octreeMinBound, octreeMaxBound };
    }

    int getVoxelIndex(int voxelSize, int boundsCoordinate)
    {
        return (boundsCoordinate >= 0) ? (boundsCoordinate / voxelSize) : ((boundsCoordinate - voxelSize + 1) / voxelSize);
    }

    glm::vec3 calculateOctreeIDFromOctree(Octree<HalfEdgeDS::Face*>* octree)
    {
        int voxelXIndexMinBound = getVoxelIndex(voxelXSize, octree->rootNode->getBounds().getMin().x);
        int voxelYIndexMinBound = getVoxelIndex(voxelYSize, octree->rootNode->getBounds().getMin().y);
        int voxelZIndexMinBound = getVoxelIndex(voxelZSize, octree->rootNode->getBounds().getMin().z);

        return { voxelXIndexMinBound, voxelYIndexMinBound, voxelZIndexMinBound };
    }
    */

    glm::vec3 getVoxelSize()
    {
        return glm::vec3(voxelXSize, voxelYSize, voxelZSize);
    }

public:
    //define sizeOfVoxels in X,Y,Z
    float voxelXSize = 25.0f;
    float voxelYSize = 25.0f;
    float voxelZSize = 25.0f;
};

export class SceneUtilities
{
public:

    static std::pair<glm::vec3, glm::vec3> calculateOctreeBounds(const glm::vec3& voxelIndex, Scene* scene) {
        glm::vec3 octreeMinBound(voxelIndex.x * scene->getVoxelSize().x, voxelIndex.y * scene->getVoxelSize().y, voxelIndex.z * scene->getVoxelSize().z);
        glm::vec3 octreeMaxBound(octreeMinBound.x + scene->getVoxelSize().x, octreeMinBound.y + scene->getVoxelSize().y, octreeMinBound.z + scene->getVoxelSize().z);
        return { octreeMinBound, octreeMaxBound };
    }

    static glm::vec3 calculateOctreeIDFromOctree(Octree<HalfEdgeDS::Face*>* octree, Scene* scene)
    {
        //int voxelXIndexMinBound = getVoxelIndex(scene->getVoxelSize().x, octree->rootNode->getBounds().getMin().x);
        //int voxelYIndexMinBound = getVoxelIndex(scene->getVoxelSize().y, octree->rootNode->getBounds().getMin().y);
        //int voxelZIndexMinBound = getVoxelIndex(scene->getVoxelSize().z, octree->rootNode->getBounds().getMin().z);

        return getVoxelIndex(octree->rootNode->getBounds().getMin(), scene);
    }

    static bool isWholeNumber(float numerator, float denominator) {
        float result = numerator / denominator;
        return std::abs(result - std::round(result)) < std::numeric_limits<float>::epsilon();
    }

    static bool isPositive(float number) {
        return number >= 0.0f;
    }

    static glm::vec3 getVoxelIndex(const glm::vec3& bounds, Scene* scene)
    {
        glm::vec3 voxelIndex;
        
        bool isXWholeNumber = SceneUtilities::isWholeNumber(bounds.x, scene->getVoxelSize().x);
        bool isXPositiveNumber = SceneUtilities::isPositive(bounds.x);

        bool isYWholeNumber = SceneUtilities::isWholeNumber(bounds.y, scene->getVoxelSize().y);
        bool isYPositiveNumber = SceneUtilities::isPositive(bounds.y);

        bool isZWholeNumber = SceneUtilities::isWholeNumber(bounds.z, scene->getVoxelSize().z);
        bool isZPositiveNumber = SceneUtilities::isPositive(bounds.z);

        //x
        if(isXPositiveNumber)
        {
            if (bounds.x == 0.0f)
            {
                voxelIndex.x = 0.0f;
            }
            else
            {
                if (isXWholeNumber)
                {
                    //-1
                    voxelIndex.x = static_cast<int>(bounds.x / scene->getVoxelSize().x) - 1;
                }
                else
                {
                    voxelIndex.x = static_cast<int>(bounds.x / scene->getVoxelSize().x);
                }
            }
        } else
        {
            if (isXWholeNumber)
            {
                voxelIndex.x = static_cast<int>(bounds.x / scene->getVoxelSize().x);
            }
            else
            {
                //-1
                voxelIndex.x = static_cast<int>(bounds.x / scene->getVoxelSize().x) - 1;
            }
        }

        //y
        if (isYPositiveNumber)
        {
            if (bounds.y == 0.0f)
            {
                voxelIndex.y = 0.0f;
            }
            else
            {
                if (isYWholeNumber)
                {
                    //-1
                    voxelIndex.y = static_cast<int>(bounds.y / scene->getVoxelSize().y) - 1;
                }
                else
                {
                    voxelIndex.y = static_cast<int>(bounds.y / scene->getVoxelSize().y);
                }
            }
        }
        else
        {
            if (isYWholeNumber)
            {
                voxelIndex.y = static_cast<int>(bounds.y / scene->getVoxelSize().y);
            }
            else
            {
                //-1
                voxelIndex.y = static_cast<int>(bounds.y / scene->getVoxelSize().y) - 1;
            }
        }


        //z
        if (isZPositiveNumber)
        {
            if (bounds.z == 0.0f)
            {
                voxelIndex.z = 0.0f;
            }
            else
            {
                if (isZWholeNumber)
                {
                    //-1
                    voxelIndex.z = static_cast<int>(bounds.z / scene->getVoxelSize().z) - 1;
                }
                else
                {
                    voxelIndex.z = static_cast<int>(bounds.z / scene->getVoxelSize().z);
                }
            }
        }
        else
        {
            if (isZWholeNumber)
            {
                voxelIndex.z = static_cast<int>(bounds.z / scene->getVoxelSize().z);
            }
            else
            {
                //-1
                voxelIndex.z = static_cast<int>(bounds.z / scene->getVoxelSize().z) - 1;
            }
        }


        return voxelIndex;
    }
};