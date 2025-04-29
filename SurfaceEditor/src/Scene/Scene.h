#include "SceneRendererData.h"
#include "../NumberUtils.h"
#include "../Mesh.h"
#include "../AABBBoundingRegion.h"
#include "../Scene/Camera.h"
#include "DataStructures/Octree.h"

class Scene
{
public:
	SceneRendererData m_rendererData;

	using FaceOctreeCoordsMap = std::map<HalfEdgeDS::Face*, std::vector<glm::vec3>>;
	using MeshFaceOctreeCoordsMap = std::map<Mesh*, FaceOctreeCoordsMap>;

	using MeshFacePair = std::pair<Mesh*, HalfEdgeDS::Face*>;
	using CoordsOctreeMap = std::map<glm::vec3, Octree<MeshFacePair>>;

	MeshFaceOctreeCoordsMap meshFaceOctreeCoordsMap;
	CoordsOctreeMap coordsOctreeMap;

	Scene(float xSize, float ySize, float zSize)
		: voxelXSize(xSize), voxelYSize(ySize), voxelZSize(zSize) {}

	~Scene() {}
	glm::vec3 getVoxelSize()
	{
		return glm::vec3(voxelXSize, voxelYSize, voxelZSize);
	}

	void deleteMeshFromOctrees(Mesh* mesh)
	{
		auto meshFaceOctreeCoordsMapIt = meshFaceOctreeCoordsMap.find(mesh);
		FaceOctreeCoordsMap& faceOctreeCoordsMap = meshFaceOctreeCoordsMapIt->second;

		for (auto& [face, octreeCoords] : faceOctreeCoordsMap)
		{
			for (glm::vec3 octreeCoord : octreeCoords)
			{
				auto coordsOctreeMapIt = coordsOctreeMap.find(octreeCoord);
				Octree<MeshFacePair>& octree = coordsOctreeMapIt->second;
				octree.removeData(std::make_pair(mesh, face));

				if (octree.isEmpty())
				{
					coordsOctreeMap.erase(coordsOctreeMapIt);
				}
			}
		}

		meshFaceOctreeCoordsMap.erase(meshFaceOctreeCoordsMapIt);
	}

	void deleteFaceFromOctrees(Mesh* mesh, HalfEdgeDS::Face* face)
	{
		auto meshFaceOctreeCoordsMapIt = meshFaceOctreeCoordsMap.find(mesh);
		auto faceOctreeCoordsMapIt = meshFaceOctreeCoordsMapIt->second.find(face);
		FaceOctreeCoordsMap& faceOctreeCoordsMap = meshFaceOctreeCoordsMapIt->second;
		std::vector<glm::vec3>& octreesCoords = faceOctreeCoordsMapIt->second;

		for (glm::vec3& coords : octreesCoords)
		{
			auto coordsOctreeMapIt = coordsOctreeMap.find(coords);
			Octree<MeshFacePair>& octree = coordsOctreeMapIt->second;
			octree.removeData(std::make_pair(mesh, face));

			if(octree.isEmpty())
			{
				//tak vieme ze mozeme tento octree cely vymazat zo sceny
				coordsOctreeMap.erase(coordsOctreeMapIt);
			}
		}

		//vymaz to aj z std::map
		faceOctreeCoordsMap.erase(faceOctreeCoordsMapIt);
		if(faceOctreeCoordsMap.empty())
		{
			meshFaceOctreeCoordsMap.erase(meshFaceOctreeCoordsMapIt);
		}
	}


private:
	float voxelXSize;
	float voxelYSize;
	float voxelZSize;
};

class SceneUtilities
{
public:

	static std::pair<glm::vec3, glm::vec3> calculateOctreeBounds(const glm::vec3& voxelIndex, Scene* scene) {
		glm::vec3 octreeMinBound(voxelIndex.x * scene->getVoxelSize().x, voxelIndex.y * scene->getVoxelSize().y, voxelIndex.z * scene->getVoxelSize().z);
		glm::vec3 octreeMaxBound(octreeMinBound.x + scene->getVoxelSize().x, octreeMinBound.y + scene->getVoxelSize().y, octreeMinBound.z + scene->getVoxelSize().z);
		return { octreeMinBound, octreeMaxBound };
	}

	static glm::vec3 calculateOctreeIDFromOctree(Octree<HalfEdgeDS::Face*>* octree, Scene* scene)
	{
		return getVoxelIndex(octree->rootNode->getBounds().getMin(), scene);
	}

	static glm::vec3 getVoxelIndex(const glm::vec3& bounds, Scene* scene)
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

		voxelIndex.x = computeVoxelIndex(bounds.x, scene->getVoxelSize().x);
		voxelIndex.y = computeVoxelIndex(bounds.y, scene->getVoxelSize().y);
		voxelIndex.z = computeVoxelIndex(bounds.z, scene->getVoxelSize().z);

		return voxelIndex;
	}
};