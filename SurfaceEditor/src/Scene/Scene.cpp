#include "Scene.h"
#include "SceneRes.h"

std::pair<SceneRes::MeshFacePair, glm::vec3> SceneUtilities::retClosestHitData(Camera* camera, Window* window, SceneRes& res)
{
	Ray ray = Ray::fromMousePos(*camera, *window);

	//---FACE_INTERSECTS_RAY---
	const auto& faceRayIntersect = [&res](const SceneRes::MeshFacePair& meshFacePair, const Ray& ray)->std::pair<bool, float>
		{
			HalfEdgeDS::Face* face = meshFacePair.second;
			Mesh* mesh = meshFacePair.first;

			SceneRes::MeshData& meshRenderData = res.meshData;

			//---MESH_VAO_MAP---
			SceneRes::MeshFacesVaoMap& meshVaoMap = meshRenderData.meshFacesVaoMap;
			//---MATERIAL_VAO_MAP---
			SceneRes::MaterialVaoMap& materialVaoMap = meshVaoMap.find(mesh)->second;
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

	std::vector<OctreeNode<SceneRes::MeshFacePair>*> octreeNodes;
	for (auto& entry : res.coordsOctreeMap)
	{
		Octree<SceneRes::MeshFacePair>& octree = entry.second;

		octree.findMaxDepthNodes<Ray>(ray, [](const AABBBoundingRegion& region, const Ray& ray)
			{
				bool intersects = region.intersectsRay(ray);
				return intersects;
			}, octreeNodes);

	}

	SceneRes::MeshFacePair closestOctreeNodeData = std::make_pair(nullptr, nullptr);
	float minRayHitDistance = std::numeric_limits<float>::min();

	for (OctreeNode<SceneRes::MeshFacePair>* node : octreeNodes)
	{
		for (SceneRes::MeshFacePair& meshFacePair : node->nodeData)
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

	return { closestOctreeNodeData, ray.origin + (ray.direction * minRayHitDistance) };
}

std::pair<glm::vec3, glm::vec3> SceneUtilities::calculateOctreeBounds(const glm::vec3& voxelIndex, const glm::vec3& voxelSize) {
	glm::vec3 octreeMinBound(voxelIndex.x * voxelSize.x, voxelIndex.y * voxelSize.y, voxelIndex.z * voxelSize.z);
	glm::vec3 octreeMaxBound(octreeMinBound.x + voxelSize.x, octreeMinBound.y + voxelSize.y, octreeMinBound.z + voxelSize.z);
	return { octreeMinBound, octreeMaxBound };
}

glm::vec3 SceneUtilities::calculateOctreeIDFromOctree(Octree<HalfEdgeDS::Face*>* octree, const glm::vec3& voxelSize)
{
	return getVoxelIndex(octree->rootNode->getBounds().getMinBoundsPos(), voxelSize);
}

glm::vec3 SceneUtilities::getVoxelIndex(const glm::vec3& bounds, const glm::vec3& voxelSize)
{
	glm::vec3 voxelIndex;

	auto computeVoxelIndex = [&](float value, float voxelSize) -> int {
		bool isWhole = NumberUtils::isWholeNumber(value, voxelSize);
		bool isPositive = NumberUtils::isPositive(value);

		int index;
		if (isPositive)
		{
			if (value == 0.0f)
				index = 0;
			else
				index = isWhole ? static_cast<int>(value / voxelSize) - 1 : static_cast<int>(value / voxelSize);
		}
		else
		{
			index = isWhole ? static_cast<int>(value / voxelSize) : static_cast<int>(value / voxelSize) - 1;
		}
		return index;
		};

	voxelIndex.x = computeVoxelIndex(bounds.x, voxelSize.x);
	voxelIndex.y = computeVoxelIndex(bounds.y, voxelSize.y);
	voxelIndex.z = computeVoxelIndex(bounds.z, voxelSize.z);

	return voxelIndex;
}