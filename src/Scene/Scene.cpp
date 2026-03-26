#include "Scene.h"
#include "../Renderer/Renderer.h"

std::pair<SceneResources::MeshFacePair, glm::vec3> SceneUtilities::retClosestHitData(Camera *camera, Window *window, SceneResources &res)
{
	Ray ray = Ray::fromMousePos(*camera, *window);

	//---FACE_INTERSECTS_RAY---
	const auto &faceRayIntersect = [&res](const SceneResources::MeshFacePair &meshFacePair, const Ray &ray) -> std::pair<bool, float>
	{
		ExtendedFace *face = meshFacePair.second;
		Mesh *mesh = meshFacePair.first;

		//---MESH_VAO_MAP---
		// potrebujeme ziskat materialVaoVertices
		//

		if (auto opt = mesh->bufferLayout.getTriangleBufferStorage(face->material))
		{
			TriangleBufferStorage &triangleBufferStorage = opt->get();
			std::vector<BufferStorageDataType::TriangleVertex> &triangleBufferVertices = triangleBufferStorage.data.vertices;

			std::vector<FaceTriangle> &faceTriangles = mesh->m_halfEdgeStructure->m_faceTriangles.find(face->material)->second;

			//---WE NEED TO CHECK WITH THE VAO DATA OF THE FACE, IF WE ACTUALLY HIT THE FACE---
			for (FaceTriangleIndex triangleIndex : face->faceTriangleIndices)
			{
				int vaoStartIndex = faceTriangles[triangleIndex].indexInVAO;

				// GET VERTICES OF TRIANGLE
				const glm::vec3 &firstVertex = triangleBufferVertices[vaoStartIndex].position;
				const glm::vec3 &secondVertex = triangleBufferVertices[vaoStartIndex + 1].position;
				const glm::vec3 &thirdVertex = triangleBufferVertices[vaoStartIndex + 2].position;

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

	std::vector<OctreeNode<SceneResources::MeshFacePair> *> octreeNodes;
	for (auto &entry : res.coordsOctreeMap)
	{
		Octree<SceneResources::MeshFacePair> &octree = entry.second;

		octree.findMaxDepthNodes<Ray>(ray, [](const AABBBoundingRegion &region, const Ray &ray)
									  {
				bool intersects = region.intersectsRay(ray);
				return intersects; }, octreeNodes);
	}

	SceneResources::MeshFacePair closestOctreeNodeData = std::make_pair(nullptr, nullptr);
	float minRayHitDistance = std::numeric_limits<float>::min();

	for (OctreeNode<SceneResources::MeshFacePair> *node : octreeNodes)
	{
		for (SceneResources::MeshFacePair &meshFacePair : node->nodeData)
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

	return {closestOctreeNodeData, ray.origin + (ray.direction * minRayHitDistance)};
}

std::pair<glm::vec3, glm::vec3> SceneUtilities::calculateOctreeBounds(const glm::vec3 &voxelIndex, const glm::vec3 &voxelSize)
{
	glm::vec3 octreeMinBound(voxelIndex.x * voxelSize.x, voxelIndex.y * voxelSize.y, voxelIndex.z * voxelSize.z);
	glm::vec3 octreeMaxBound(octreeMinBound.x + voxelSize.x, octreeMinBound.y + voxelSize.y, octreeMinBound.z + voxelSize.z);
	return {octreeMinBound, octreeMaxBound};
}

glm::vec3 SceneUtilities::calculateOctreeIDFromOctree(Octree<ExtendedFace *> *octree, const glm::vec3 &voxelSize)
{
	return getVoxelIndex(octree->rootNode->getBounds().getMinBoundsPos(), voxelSize);
}

glm::vec3 SceneUtilities::getVoxelIndex(const glm::vec3 &bounds, const glm::vec3 &voxelSize)
{
	glm::vec3 voxelIndex;

	auto computeVoxelIndex = [&](float value, float voxelSize) -> int
	{
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

Scene::Scene(float xSize, float ySize, float zSize)
{
	m_voxelSize.x = xSize;
	m_voxelSize.y = ySize;
	m_voxelSize.z = zSize;
}

glm::vec3 Scene::getVoxelSize()
{
	return m_voxelSize;
}

void Scene::addFaceIntoOctrees(Mesh *mesh, ExtendedFace *face)
{
	SceneResources::FaceOctreeCoordsMap &faceOctreeCoordsMap = m_res.meshFaceOctreeCoordsMap[mesh];

	// COMPUTE BOUNDING BOX OF THE FACE
	std::vector<ExtendedVertex> faceVerts;
	mesh->m_halfEdgeStructure->getVerticesFromFace(face, faceVerts);

	AABBBoundingRegion faceBounds(faceVerts.begin(), faceVerts.end(),
								  [](ExtendedVertex &point)
								  { return point.m_position; });

	glm::vec3 sceneVoxelMinCoords = SceneUtilities::getVoxelIndex(faceBounds.getMinBoundsPos(), m_voxelSize);
	glm::vec3 sceneVoxelMaxCoords = SceneUtilities::getVoxelIndex(faceBounds.getMaxBoundsPos(), m_voxelSize);

	for (int x = sceneVoxelMinCoords.x; x <= sceneVoxelMaxCoords.x; ++x)
	{
		for (int y = sceneVoxelMinCoords.y; y <= sceneVoxelMaxCoords.y; ++y)
		{
			for (int z = sceneVoxelMinCoords.z; z <= sceneVoxelMaxCoords.z; ++z)
			{
				glm::vec3 currentVoxelBounds = {x, y, z};

				Octree<SceneResources::MeshFacePair> *insertionOctree = retrieveCreatedOctree(currentVoxelBounds);

				insertionOctree->addDataToOctree(SceneResources::MeshFacePair(mesh, face), faceBounds);

				// insertionOctree->addDataToOctree(Scene::MeshFacePair(mesh, &(*faceIter)), faceBounds);

				// ADD FACE INTO THE MESH_FACE_OCTREE_COORDS_MAP
				faceOctreeCoordsMap[face].push_back(currentVoxelBounds);
				// faceOctreeCoordsMap[&(*faceIter)].push_back(currentVoxelBounds);
			}
		}
	}
}

void Scene::deleteMeshFromOctrees(Mesh *mesh)
{
	auto meshFaceOctreeCoordsMapIt = m_res.meshFaceOctreeCoordsMap.find(mesh);
	SceneResources::FaceOctreeCoordsMap &faceOctreeCoordsMap = meshFaceOctreeCoordsMapIt->second;

	for (auto &[face, octreeCoords] : faceOctreeCoordsMap)
	{
		for (glm::vec3 octreeCoord : octreeCoords)
		{
			auto coordsOctreeMapIt = m_res.coordsOctreeMap.find(octreeCoord);
			Octree<SceneResources::MeshFacePair> &octree = coordsOctreeMapIt->second;
			octree.removeData(std::make_pair(mesh, face));

			if (octree.isEmpty())
			{
				m_res.coordsOctreeMap.erase(coordsOctreeMapIt);
			}
		}
	}

	m_res.meshFaceOctreeCoordsMap.erase(meshFaceOctreeCoordsMapIt);
}

void Scene::deleteFaceFromOctrees(Mesh *mesh, ExtendedFace *face)
{
	auto meshFaceOctreeCoordsMapIt = m_res.meshFaceOctreeCoordsMap.find(mesh);
	auto faceOctreeCoordsMapIt = meshFaceOctreeCoordsMapIt->second.find(face);
	SceneResources::FaceOctreeCoordsMap &faceOctreeCoordsMap = meshFaceOctreeCoordsMapIt->second;
	std::vector<glm::vec3> &octreesCoords = faceOctreeCoordsMapIt->second;

	for (glm::vec3 &coords : octreesCoords)
	{
		auto coordsOctreeMapIt = m_res.coordsOctreeMap.find(coords);
		Octree<SceneResources::MeshFacePair> &octree = coordsOctreeMapIt->second;
		octree.removeData(std::make_pair(mesh, face));

		if (octree.isEmpty())
		{
			// tak vieme ze mozeme tento octree cely vymazat zo sceny
			m_res.coordsOctreeMap.erase(coordsOctreeMapIt);
		}
	}

	// vymaz to aj z std::map
	faceOctreeCoordsMap.erase(faceOctreeCoordsMapIt);
	if (faceOctreeCoordsMap.empty())
	{
		m_res.meshFaceOctreeCoordsMap.erase(meshFaceOctreeCoordsMapIt);
	}
}

Octree<SceneResources::MeshFacePair> *Scene::retrieveCreatedOctree(glm::vec3 octreeVoxelBounds)
{
	SceneResources::CoordsOctreeMap::iterator coordsOctreeMapIt = m_res.coordsOctreeMap.find(octreeVoxelBounds);

	bool isOctreeCreated = coordsOctreeMapIt != m_res.coordsOctreeMap.end();
	Octree<SceneResources::MeshFacePair> *retOctree = nullptr;

	// FIND IF THE OCTREE ON THAT VOXEL AREA EXISTS
	if (!isOctreeCreated)
	{
		// CREATE NEW OCTREE ON THAT VOXEL COORDS
		auto [octreeMinBounds, octreeMaxBounds] = SceneUtilities::calculateOctreeBounds(octreeVoxelBounds, m_voxelSize);

		// RETRIEVE NEWLY CREATED OCTREE
		retOctree = &m_res.coordsOctreeMap.emplace(
											  octreeVoxelBounds, Octree<SceneResources::MeshFacePair>(octreeMinBounds, octreeMaxBounds))
						 .first->second;
	}
	else
	{
		// RETRIEVE ALREADY EXISTING OCTREE
		retOctree = &coordsOctreeMapIt->second;
	}

	return retOctree;
}
