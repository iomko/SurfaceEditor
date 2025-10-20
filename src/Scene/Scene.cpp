#include "Scene.h"
#include "../Renderer/Renderer.h"

std::vector<SceneResources::MeshFacePair> SceneUtilities::retAllHitDataInBoxSelection(Camera* camera, Window* window, SceneResources& res, const glm::vec2& startMousePos, const glm::vec2& endMousePos)
{
	std::vector<SceneResources::MeshFacePair> hitDataInBoxSelection;
	
	glm::vec2 min = glm::min(startMousePos, endMousePos);
    glm::vec2 max = glm::max(startMousePos, endMousePos);

	auto isInScreenBox = [&](const glm::vec3& worldPos) -> bool {
        glm::vec4 clipPos = camera->m_matrices.perspectiveMatrix * camera->m_matrices.viewMatrix * glm::vec4(worldPos, 1.0f);
        if (clipPos.w == 0.0f) return false;
        glm::vec3 ndc = glm::vec3(clipPos) / clipPos.w; // Normalized Device Coordinates (-1..1)
        glm::vec2 screenPos;
        screenPos.x = (ndc.x * 0.5f + 0.5f) * window->getScreenWidth();
        screenPos.y = (1.0f - (ndc.y * 0.5f + 0.5f)) * window->getScreenHeight(); // y-flip

        return screenPos.x >= min.x && screenPos.x <= max.x &&
               screenPos.y >= min.y && screenPos.y <= max.y;
    };

	for (auto& entry : res.coordsOctreeMap)
    {
        Octree<SceneResources::MeshFacePair>& octree = entry.second;
        std::vector<OctreeNode<SceneResources::MeshFacePair>*> nodes;

		//find octree nodes
		
		printf("Found %zu octree nodes in box selection\n", nodes.size()); // --- IGNORE ---
		for (OctreeNode<SceneResources::MeshFacePair>* node : nodes)
        {
            for (auto& meshFacePair : node->nodeData)
            {
                ExtendedFace* face = meshFacePair.second;
                Mesh* mesh = meshFacePair.first;

                if (auto opt = mesh->bufferLayout.getTriangleBufferStorage(face->material))
                {
                    auto& storage = opt->get();
                    auto& vertices = storage.data.vertices;
                    auto& faceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles[face->material];

                    for (auto triIndex : face->faceTriangleIndices)
                    {
                        int idx = faceTriangles[triIndex].indexInVAO;
                        glm::vec3 v1 = vertices[idx].position;
                        glm::vec3 v2 = vertices[idx + 1].position;
                        glm::vec3 v3 = vertices[idx + 2].position;

                        if (isInScreenBox(v1) || isInScreenBox(v2) || isInScreenBox(v3))
                        {
                            hitDataInBoxSelection.push_back(meshFacePair);
                            break;
                        }
                    }
                }
            }
	    }		
	}
	return hitDataInBoxSelection;
}

std::pair<SceneResources::MeshFacePair, glm::vec3> SceneUtilities::retClosestHitData(Camera* camera, Window* window, SceneResources& res)
{
	Ray ray = Ray::fromMousePos(*camera, *window);

	//---FACE_INTERSECTS_RAY---
	const auto& faceRayIntersect = [&res](const SceneResources::MeshFacePair& meshFacePair, const Ray& ray)->std::pair<bool, float>
		{
			ExtendedFace* face = meshFacePair.second;
			Mesh* mesh = meshFacePair.first;

			//---MESH_VAO_MAP---
            //potrebujeme ziskat materialVaoVertices
            //

            if (auto opt = mesh->bufferLayout.getTriangleBufferStorage(face->material)) {
                TriangleBufferStorage& triangleBufferStorage = opt->get();
                std::vector<BufferStorageDataType::TriangleVertex>& triangleBufferVertices = triangleBufferStorage.data.vertices;


                std::vector<FaceTriangle>& faceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second;

                //---WE NEED TO CHECK WITH THE VAO DATA OF THE FACE, IF WE ACTUALLY HIT THE FACE---
                for (FaceTriangleIndex triangleIndex : face->faceTriangleIndices)
                {
                    int vaoStartIndex = faceTriangles[triangleIndex].indexInVAO;

                    //GET VERTICES OF TRIANGLE
                    const glm::vec3& firstVertex = triangleBufferVertices[vaoStartIndex].position;
                    const glm::vec3& secondVertex = triangleBufferVertices[vaoStartIndex + 1].position;
                    const glm::vec3& thirdVertex = triangleBufferVertices[vaoStartIndex + 2].position;

                    float rayMultiplyAmount;
                    if (Ray::intersectsTriangle(ray.origin, ray.direction, firstVertex, secondVertex, thirdVertex, rayMultiplyAmount))
                    {
                        return std::make_pair(true, rayMultiplyAmount);
                    }
                }

                return std::make_pair(false, -1.0f);

            }
            return std::make_pair(false, -1.0f);

		};

	std::vector<OctreeNode<SceneResources::MeshFacePair>*> octreeNodes;
	for (auto& entry : res.coordsOctreeMap)
	{
		Octree<SceneResources::MeshFacePair>& octree = entry.second;

		octree.findMaxDepthNodes<Ray>(ray, [](const AABBBoundingRegion& region, const Ray& ray)
			{
				bool intersects = region.intersectsRay(ray);
				return intersects;
			}, octreeNodes);

	}

	SceneResources::MeshFacePair closestOctreeNodeData = std::make_pair(nullptr, nullptr);
	float minRayHitDistance = std::numeric_limits<float>::min();

	for (OctreeNode<SceneResources::MeshFacePair>* node : octreeNodes)
	{
		for (SceneResources::MeshFacePair& meshFacePair : node->nodeData)
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

glm::vec3 SceneUtilities::calculateOctreeIDFromOctree(Octree<ExtendedFace*>* octree, const glm::vec3& voxelSize)
{
	return getVoxelIndex(octree->rootNode->getBounds().getMinBoundsPos(), voxelSize);
}

glm::vec3 SceneUtilities::getVoxelIndex(const glm::vec3& bounds, const glm::vec3& voxelSize)
{
	glm::vec3 voxelIndex;

	auto computeVoxelIndex = [&](float value, float voxelSize) -> int {
		bool isWhole = std::fabs(std::fmod(value, voxelSize)) < 1e-6f;
		bool isPositive = value >= 0.0f;

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
