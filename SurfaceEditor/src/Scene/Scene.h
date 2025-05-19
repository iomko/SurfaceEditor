#include "SceneRendererData.h"
#include "../NumberUtils.h"
#include "../Mesh.h"
#include "../AABBBoundingRegion.h"
#include "../Scene/Camera.h"
#include "DataStructures/Octree.h"

class SceneUtilities
{
public:
	static std::pair<glm::vec3, glm::vec3> calculateOctreeBounds(const glm::vec3& voxelIndex, const glm::vec3& voxelSize) {
		glm::vec3 octreeMinBound(voxelIndex.x * voxelSize.x, voxelIndex.y * voxelSize.y, voxelIndex.z * voxelSize.z);
		glm::vec3 octreeMaxBound(octreeMinBound.x + voxelSize.x, octreeMinBound.y + voxelSize.y, octreeMinBound.z + voxelSize.z);
		return { octreeMinBound, octreeMaxBound };
	}

	static glm::vec3 calculateOctreeIDFromOctree(Octree<HalfEdgeDS::Face*>* octree, const glm::vec3& voxelSize)
	{
		return getVoxelIndex(octree->rootNode->getBounds().getMin(), voxelSize);
	}

	static glm::vec3 getVoxelIndex(const glm::vec3& bounds, const glm::vec3& voxelSize)
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
};

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

	/*
	Scene(float xSize, float ySize, float zSize)
		: voxelXSize(xSize), voxelYSize(ySize), voxelZSize(zSize) {}
	*/

	Scene(float xSize, float ySize, float zSize)
	{
		m_voxelSize.x = xSize;
		m_voxelSize.y = ySize;
		m_voxelSize.z = zSize;
	}

	~Scene() {}
	glm::vec3 getVoxelSize()
	{
		return m_voxelSize;
	}

	void addFaceIntoOctrees(Mesh* mesh, HalfEdgeDS::Face* face)
	{
		FaceOctreeCoordsMap& faceOctreeCoordsMap = meshFaceOctreeCoordsMap[mesh];

		//COMPUTE BOUNDING BOX OF THE FACE
		std::vector<HalfEdgeDS::Vertex> faceVerts;
		mesh->m_halfEdgeStructure->getVerticesFromFace(face, faceVerts);
		AABBBoundingRegion faceBounds(
			faceVerts.begin(),
			faceVerts.end(),
			[](HalfEdgeDS::Vertex& p) { return p.m_position.x; },
			[](HalfEdgeDS::Vertex& p) { return p.m_position.y; },
			[](HalfEdgeDS::Vertex& p) { return p.m_position.z; }
		);

		glm::vec3 sceneVoxelMinCoords = SceneUtilities::getVoxelIndex(faceBounds.getMin(), m_voxelSize);
		glm::vec3 sceneVoxelMaxCoords = SceneUtilities::getVoxelIndex(faceBounds.getMax(), m_voxelSize);

		for (int x = sceneVoxelMinCoords.x; x <= sceneVoxelMaxCoords.x; ++x)
		{
			for (int y = sceneVoxelMinCoords.y; y <= sceneVoxelMaxCoords.y; ++y)
			{
				for (int z = sceneVoxelMinCoords.z; z <= sceneVoxelMaxCoords.z; ++z)
				{
					glm::vec3 currentVoxelBounds = { x,y,z };

					Octree<MeshFacePair>* insertionOctree = retrieveCreatedOctree(currentVoxelBounds);

					insertionOctree->addDataToOctree(MeshFacePair(mesh, face), faceBounds);

					//insertionOctree->addDataToOctree(Scene::MeshFacePair(mesh, &(*faceIter)), faceBounds);

					//ADD FACE INTO THE MESH_FACE_OCTREE_COORDS_MAP
					faceOctreeCoordsMap[face].push_back(currentVoxelBounds);
					//faceOctreeCoordsMap[&(*faceIter)].push_back(currentVoxelBounds);
				}
			}

		}
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

	Octree<Scene::MeshFacePair>* retrieveCreatedOctree(glm::vec3 octreeVoxelBounds)
	{
		CoordsOctreeMap::iterator coordsOctreeMapIt = coordsOctreeMap.find(octreeVoxelBounds);

		bool isOctreeCreated = coordsOctreeMapIt != coordsOctreeMap.end();
		Octree<MeshFacePair>* retOctree = nullptr;

		//FIND IF THE OCTREE ON THAT VOXEL AREA EXISTS
		if (!isOctreeCreated)
		{
			//CREATE NEW OCTREE ON THAT VOXEL COORDS
			auto [octreeMinBounds, octreeMaxBounds] = SceneUtilities::calculateOctreeBounds(octreeVoxelBounds, m_voxelSize);

			//RETRIEVE NEWLY CREATED OCTREE
			retOctree = &coordsOctreeMap.emplace(
				octreeVoxelBounds, Octree<Scene::MeshFacePair>(octreeMinBounds, octreeMaxBounds)).first->second;
		}
		else
		{
			//RETRIEVE ALREADY EXISTING OCTREE
			retOctree = &coordsOctreeMapIt->second;
		}

		return retOctree;
	}

	glm::vec3 m_voxelSize;
	//float voxelXSize;
	//float voxelYSize;
	//float voxelZSize;
};