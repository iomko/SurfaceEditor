#pragma once
#include <iostream>

#include "../Callback.h"
#include "../ViewPortsHolder.h"
#include "../AABBBoundingRegion.h"
#include "../Core/Input.h"

class SelectMeshCallBack : public Callback, public Observer
{
public:
	virtual void execute() override
	{
		//select Mesh
        Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;

        

		glm::vec3 cameraDirection = ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_activeCamera->getState().frontVector;
		glm::vec3 cameraPosition = ViewPortsHolderContext::m_viewPortsHolder->m_viewPortLayers.at(0)->m_activeCamera->getState().position;

		Ray cameraRay(cameraPosition, cameraDirection);



        const auto& octreeAlg = [](const std::pair<Mesh*, HalfEdgeDS::Face*>& faceMeshPair, const Ray& cameraRay)->std::pair<bool, float>
            {
                size_t faceIndex = faceMeshPair.second->getHalfEdge()->getFaceIndex();
                //size_t faceIndex = std::distance(faceMeshPair.first->getMeshData()->m_faces.begin(), faceMeshPair.second->getHalfEdge()->getFace());
                size_t faceStartIndex = faceMeshPair.first->m_facesData.at(faceIndex).startIndex;
                size_t faceEndIndex = faceMeshPair.first->m_facesData.at(faceIndex).endIndex;
                for (int i = faceStartIndex; i < faceEndIndex; i += 3)
                {
                    //musime vytvorit trojuholniky
                    const auto& firstVertex = faceMeshPair.first->m_combinedVertexDataMatVector.m_vertexData.at(i).position;
                    const auto& secondVertex = faceMeshPair.first->m_combinedVertexDataMatVector.m_vertexData.at(i + 1).position;
                    const auto& thirdVertex = faceMeshPair.first->m_combinedVertexDataMatVector.m_vertexData.at(i + 2).position;

                    float amountToBeMultiplied;

                    if(Ray::intersectsTriangle(cameraRay.origin, cameraRay.direction, firstVertex, secondVertex, thirdVertex, amountToBeMultiplied))
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
        for (auto& meshFaceOctree : scene->m_meshFaceOctreesMap)
        {
            const auto& octreeRetData = meshFaceOctree.second.findDataInOctree(cameraRay, octreeAlg);

            const auto& octreeNode = std::get<0>(octreeRetData);
            const auto& mesh = std::get<1>(octreeRetData).first;
            const auto& distance = std::get<2>(octreeRetData);

            if (octreeNode != nullptr)
            {
                if (distance < minOctreeHitDistance)
                {
                    retMesh = mesh;
                    minOctreeHitDistance = distance;
                }
            }
        }

        if(retMesh != nullptr)
        {
            //to znamena ze sme na nejaky mesh klikli
            ViewPortsHolderContext::m_viewPortsHolder->m_selectedMeshes.insert(retMesh);

        } else
        {
            //vtedy vymazem vsetky selectedMeshes
            ViewPortsHolderContext::m_viewPortsHolder->m_selectedMeshes.clear();
        }



        m_selectedMesh = retMesh;
        //notifyObservers();
	}
public:
    Mesh* m_selectedMesh = nullptr;
};
