module;
#include "../NumberUtils.h"
#include "../Mesh.h"
#include "../AABBBoundingRegion.h"
#include "../Scene/Camera.h"
export module Scene;

import DataStructures;


export class Scene
{
public:
	std::map <Mesh*, std::map<HalfEdgeDS::Face*, std::vector<glm::vec3>>> m_meshesFaceOctreeMap;
	std::map <glm::vec3, Octree<std::pair<Mesh*, HalfEdgeDS::Face*>>> m_meshFaceOctreesMap;

	Scene(float xSize, float ySize, float zSize)
		: voxelXSize(xSize), voxelYSize(ySize), voxelZSize(zSize) {}

	~Scene() {}
	glm::vec3 getVoxelSize()
	{
		return glm::vec3(voxelXSize, voxelYSize, voxelZSize);
	}

private:
	float voxelXSize;
	float voxelYSize;
	float voxelZSize;
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