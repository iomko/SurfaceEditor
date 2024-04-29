#pragma once
#include <iostream>

#include "../Callback.h"
#include "../ViewPortHolder.h"
#include "../AABBBoundingRegion.h"
#include "../Core/Input.h"

class SelectMeshCallBack : public Callback, public Observable
{
public:
	SelectMeshCallBack(ViewPortHolder* viewPortHolder)
	{
		m_viewPortHolder = viewPortHolder;
	}

	void execute() override
	{

		//select Mesh
		Scene* scene = m_viewPortHolder->m_viewPortLayer->getScene();

        Ray ray = m_viewPortHolder->m_viewPortLayer->getCamera()->m_cameraRay;



            const auto& octreeAlg = [](const std::pair<Mesh*, HalfEdgeDS::Face*>& faceMeshPair, const Ray& ray)->std::pair<bool, float>
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
            for (auto& meshFaceOctree : scene->m_meshFaceOctreesMap)
            {
                const auto& octreeRetData = meshFaceOctree.second.findDataInOctree(ray, octreeAlg);

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
                m_viewPortHolder->m_selectedMeshes.insert(retMesh);

            } else
            {
                //vtedy vymazem vsetky selectedMeshes
                m_viewPortHolder->m_selectedMeshes.clear();
            }



            m_selectedMesh = retMesh;
            notifyObservers();
	}
public:
    Mesh* m_selectedMesh = nullptr;
	ViewPortHolder* m_viewPortHolder;
};
