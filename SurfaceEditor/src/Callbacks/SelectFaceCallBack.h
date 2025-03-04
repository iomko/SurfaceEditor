#pragma once
#include "Callback.h"
#include "../Commands/CmdProperties/CmdProperties.h"
#include "../Mesh.h"
#include "../ViewPortsHolder.h"

class SelectFaceCallBack : public Callback
{
public:
	void execute(const Params& inputParams, Params& outputParams) override
	{
		const SelectFaceParamsIn& castedInputParams = static_cast<const SelectFaceParamsIn&>(inputParams);

		const auto& octreeAlg = [](const std::pair<Mesh*, HalfEdgeDS::Face*>& faceMeshPair, const Ray& ray)->std::pair<bool, float>
			{
				//size_t faceIndex = std::distance(faceMeshPair.first->getHalfEdgeStructure()->m_faces.begin(), faceMeshPair.second->getHalfEdge()->getFace());
				HalfEdgeDS::Face* currentFace = faceMeshPair.second;
				Mesh* currentMesh = faceMeshPair.first;

				//---SCENERENDERERDATA---
				Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;
				SceneRendererData::MeshFacesMap& meshFacesMap = scene->m_rendererData.meshData.meshFacesMap;
				SceneRendererData::MeshFacesMap::iterator meshFacesMapIt = meshFacesMap.find(currentMesh);

				SceneRendererData::MeshVaoDataMap& meshVaoDataMap = scene->m_rendererData.meshData.meshVaoDataMap;
				SceneRendererData::MeshVaoDataMap::iterator meshVaoDataMapIt = meshVaoDataMap.find(currentMesh);
				//---SCENERENDERERDATA---

				//---GETTING THE FACEINFO OF THE CURRENT FACE---
				SceneRendererData::FaceInfoMap& faceInfoMap = meshFacesMapIt->second;
				SceneRendererData::FaceInfoMap::iterator faceInfoMapIt = faceInfoMap.find(currentFace->getHalfEdge().operator*().getFaceIndex());
				SceneRendererData::FaceInfo& faceInfo = faceInfoMapIt->second;


				//---GETTING THE FACES VAO DATA---
				SceneRendererData::MaterialVertexMap& materialVertexMap = meshVaoDataMapIt->second;
				SceneRendererData::MaterialVertexMap::iterator materialVertexMapIt = materialVertexMap.find(faceInfo.material);
				std::vector<MeshVertex>& facesVerticesVector = materialVertexMapIt->second;


				size_t faceStartIndex = faceInfo.startIndex;
				size_t faceEndIndex = faceInfo.endIndex;

				std::cout << "FaceStartIndex: " << faceInfo.startIndex << std::endl;
				std::cout << "FaceEndIndex: " << faceInfo.endIndex << std::endl;

				//---WE NEED TO CHECK WITH THE VAO DATA OF THE FACE, IF WE ACTUALLY HIT THE FACE---
				for (int i = faceStartIndex; i < faceEndIndex; i += 3)
				{
					//GET VERTICES OF TRIANGLE
					const auto& firstVertex = facesVerticesVector.at(i).position;
					const auto& secondVertex = facesVerticesVector.at(i + 1).position;
					const auto& thirdVertex = facesVerticesVector.at(i + 2).position;

					float amountToBeMultiplied;
					if (Ray::intersectsTriangle(ray.origin, ray.direction, firstVertex, secondVertex, thirdVertex, amountToBeMultiplied))
					{
						return std::make_pair(true, amountToBeMultiplied);
					}

				}
				return std::make_pair(false, -1.0f);
			};

		float minOctreeHitDistance = std::numeric_limits<float>::max();
		HalfEdgeDS::Face* retFace = nullptr;
		Mesh* retMesh = nullptr;

		for (auto& meshFaceOctree : ViewPortsHolderContext::m_viewPortsHolder->m_scene->coordsOctreeMap)
		{

			std::vector<OctreeNode<Scene::MeshFacePair>*> accumulatedOctreeNodes;

			auto octreeBoundsIntersectAlg = [](const AABBBoundingRegion& octreeNodeBounds, const Ray& ray) -> bool {
				return octreeNodeBounds.intersectsRay(ray);
				};

			meshFaceOctree.second.findNodesInOctree<Ray>(castedInputParams.ray, octreeBoundsIntersectAlg, accumulatedOctreeNodes);

			//teraz budem musiet prechadzat cez vsetky tieto octreeNodes
			//musime najst najblizsi face a vratit aj jeho octreeNode

			HalfEdgeDS::Face* closestFace = nullptr;
			Mesh* closestMesh = nullptr;
			OctreeNode<Scene::MeshFacePair>* closestNode = nullptr;
			float minHitDistance = std::numeric_limits<float>::max();

			for (OctreeNode<Scene::MeshFacePair>* octreeNode : accumulatedOctreeNodes)
			{
				for (Scene::MeshFacePair& data : octreeNode->nodeData)
				{
					const auto& distanceHitPair = octreeAlg(data, castedInputParams.ray);
					if (distanceHitPair.first)
					{
						if (distanceHitPair.second < minHitDistance)
						{
							minHitDistance = distanceHitPair.second;
							closestMesh = data.first;
							closestFace = data.second;
							closestNode = octreeNode;
						}

					}

				}
			}

			if (closestNode != nullptr)
			{
				retMesh = closestMesh;
				retFace = closestFace;
				minOctreeHitDistance = minHitDistance;
			}

		}


		SelectFaceParamsOut& castedOutputParams = static_cast<SelectFaceParamsOut&>(outputParams);

		castedOutputParams.mesh = retMesh;
		castedOutputParams.face = retFace;
		castedOutputParams.hitPoint = castedInputParams.ray.origin + (glm::normalize(castedInputParams.ray.direction) * minOctreeHitDistance);

	}
};