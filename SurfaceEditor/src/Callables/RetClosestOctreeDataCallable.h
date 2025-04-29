#pragma once
#include "Callable.h"
#include "../ViewPortsHolder.h"
#include "../Commands/CmdProperties/CmdProperties.h"

class RetClosestOctreeDataCallable : public Callable<void, OctreeNodeDataParams>
{
public:
	void invoke(OctreeNodeDataParams& output) override
	{

		Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;
		Ray ray = Ray::fromMousePos(*ViewPortsHolderContext::m_camera, ViewPortsHolderContext::m_camera->m_matrices.perspectiveMatrix,
			ViewPortsHolderContext::m_camera->m_matrices.viewMatrix, *ViewPortsHolderContext::m_window);

		//---FACE_INTERSECTS_RAY---
		const auto& faceRayIntersect = [](const Scene::MeshFacePair& meshFacePair, const Ray& ray)->std::pair<bool, float>
			{
				HalfEdgeDS::Face* face = meshFacePair.second;
				Mesh* mesh = meshFacePair.first;
				Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;

				SceneRendererData::MeshData& meshRenderData = scene->m_rendererData.meshData;

				//---MESH_VAO_MAP---
				SceneRendererData::MeshFacesVaoMap& meshVaoMap = meshRenderData.meshFacesVaoMap;
				//---MATERIAL_VAO_MAP---
				SceneRendererData::MaterialVaoMap& materialVaoMap = meshVaoMap.find(mesh)->second;
				//---VAO_DATA_OF_SPECIFIC_MATERIAL---
				std::vector<MeshVertex>& materialVaoFaces = materialVaoMap.find(face->material)->second;

				std::vector<HalfEdgeDS::FaceTriangle>& faceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second;

				//---WE NEED TO CHECK WITH THE VAO DATA OF THE FACE, IF WE ACTUALLY HIT THE FACE---
				for (FaceTriangleIndex triangleIndex : face->faceTriangleIndices)
				{
					int vaoStartIndex = faceTriangles[triangleIndex].indexInVAO;

					//GET VERTICES OF TRIANGLE
					const glm::vec3& firstVertex = materialVaoFaces[vaoStartIndex].position;
					const glm::vec3& secondVertex = materialVaoFaces[vaoStartIndex + 1].position;
					const glm::vec3& thirdVertex = materialVaoFaces[vaoStartIndex + 2].position;
					
					float rayMultiplyAmount;
					if (Ray::intersectsTriangle(ray.origin, ray.direction, firstVertex, secondVertex, thirdVertex, rayMultiplyAmount))
					{
						return std::make_pair(true, rayMultiplyAmount);
					}
				}

				return std::make_pair(false, -1.0f);
			};

		std::vector<OctreeNode<Scene::MeshFacePair>*> octreeNodes;
		for (auto& entry : scene->coordsOctreeMap)
		{
			Octree<Scene::MeshFacePair>& octree = entry.second;

			octree.findMaxDepthNodes<Ray>(ray, [](const AABBBoundingRegion& region, const Ray& ray)
			{
				bool intersects = region.intersectsRay(ray);

				if(intersects)
				{
					std::cout << "YES INTERSECTED!" << std::endl;
				}
				return intersects;
			}, octreeNodes);
			
		}

		Scene::MeshFacePair closestOctreeNodeData = std::make_pair(nullptr, nullptr);
		float minRayHitDistance = std::numeric_limits<float>::min();
		
		for (OctreeNode<Scene::MeshFacePair>* node : octreeNodes)
		{
			for (Scene::MeshFacePair& meshFacePair : node->nodeData)
			{
				std::pair<bool, float> intersects = faceRayIntersect(meshFacePair, ray);
				if (intersects.first)
				{
					if (minRayHitDistance == std::numeric_limits<float>::min() || (intersects.second < minRayHitDistance))
					{
						minRayHitDistance = intersects.second;
						closestOctreeNodeData = meshFacePair;
					}

				}

			}
		}

		output.hitPoint = ray.origin + (ray.direction * minRayHitDistance);
		output.meshFacePair = closestOctreeNodeData;
	}
};
