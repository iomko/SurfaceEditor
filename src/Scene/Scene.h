#pragma once

#include "../Scene/Mesh.h"
#include "../Primitives/AABB.h"
#include "../Scene/Camera.h"
#include "../DataStructures/Octree.h"
#include "../Core/Window.h"

struct SceneResources
{
	using FaceOctreeCoordsMap = std::map<ExtendedFace*, std::vector<glm::vec3>>;
	using MeshFaceOctreeCoordsMap = std::map<Mesh*, FaceOctreeCoordsMap>;
	using MeshFacePair = std::pair<Mesh*, ExtendedFace*>;
	using CoordsOctreeMap = std::map<glm::vec3, Octree<MeshFacePair>>;

	using MatFacesMap = std::map<Material*, std::vector<ExtendedFace*>>;
	using MeshFacesMap = std::map<Mesh*, MatFacesMap>;

	MeshFaceOctreeCoordsMap meshFaceOctreeCoordsMap;
	CoordsOctreeMap coordsOctreeMap;
};

class SceneUtilities
{
public:
	static std::pair<SceneResources::MeshFacePair, glm::vec3> retClosestHitData(Camera* camera, Window* window, SceneResources& res);
	static std::pair<glm::vec3, glm::vec3> calculateOctreeBounds(const glm::vec3& voxelIndex, const glm::vec3& voxelSize);
	static glm::vec3 calculateOctreeIDFromOctree(Octree<ExtendedFace*>* octree, const glm::vec3& voxelSize);
	static glm::vec3 getVoxelIndex(const glm::vec3& bounds, const glm::vec3& voxelSize);
};

class Scene
{
public:
	SceneResources m_res;

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

	void addFaceIntoOctrees(Mesh* mesh, ExtendedFace* face)
	{
		SceneResources::FaceOctreeCoordsMap& faceOctreeCoordsMap = m_res.meshFaceOctreeCoordsMap[mesh];

		//COMPUTE BOUNDING BOX OF THE FACE
		std::vector<ExtendedVertex> faceVerts;
		mesh->m_halfEdgeStructure->getVerticesFromFace(face, faceVerts);

		AABBBoundingRegion faceBounds(faceVerts.begin(), faceVerts.end(),
			[](ExtendedVertex& point) { return point.m_position; });

		glm::vec3 sceneVoxelMinCoords = SceneUtilities::getVoxelIndex(faceBounds.getMinBoundsPos(), m_voxelSize);
		glm::vec3 sceneVoxelMaxCoords = SceneUtilities::getVoxelIndex(faceBounds.getMaxBoundsPos(), m_voxelSize);

		for (int x = sceneVoxelMinCoords.x; x <= sceneVoxelMaxCoords.x; ++x)
		{
			for (int y = sceneVoxelMinCoords.y; y <= sceneVoxelMaxCoords.y; ++y)
			{
				for (int z = sceneVoxelMinCoords.z; z <= sceneVoxelMaxCoords.z; ++z)
				{
					glm::vec3 currentVoxelBounds = { x,y,z };

					Octree<SceneResources::MeshFacePair>* insertionOctree = retrieveCreatedOctree(currentVoxelBounds);

					insertionOctree->addDataToOctree(SceneResources::MeshFacePair(mesh, face), faceBounds);

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
		auto meshFaceOctreeCoordsMapIt = m_res.meshFaceOctreeCoordsMap.find(mesh);
		SceneResources::FaceOctreeCoordsMap& faceOctreeCoordsMap = meshFaceOctreeCoordsMapIt->second;

		for (auto& [face, octreeCoords] : faceOctreeCoordsMap)
		{
			for (glm::vec3 octreeCoord : octreeCoords)
			{
				auto coordsOctreeMapIt = m_res.coordsOctreeMap.find(octreeCoord);
				Octree<SceneResources::MeshFacePair>& octree = coordsOctreeMapIt->second;
				octree.removeData(std::make_pair(mesh, face));

				if (octree.isEmpty())
				{
					m_res.coordsOctreeMap.erase(coordsOctreeMapIt);
				}
			}
		}

		m_res.meshFaceOctreeCoordsMap.erase(meshFaceOctreeCoordsMapIt);
	}

	void deleteFaceFromOctrees(Mesh* mesh, ExtendedFace* face)
	{
		auto meshFaceOctreeCoordsMapIt = m_res.meshFaceOctreeCoordsMap.find(mesh);
		auto faceOctreeCoordsMapIt = meshFaceOctreeCoordsMapIt->second.find(face);
		SceneResources::FaceOctreeCoordsMap& faceOctreeCoordsMap = meshFaceOctreeCoordsMapIt->second;
		std::vector<glm::vec3>& octreesCoords = faceOctreeCoordsMapIt->second;

		for (glm::vec3& coords : octreesCoords)
		{
			auto coordsOctreeMapIt = m_res.coordsOctreeMap.find(coords);
			Octree<SceneResources::MeshFacePair>& octree = coordsOctreeMapIt->second;
			octree.removeData(std::make_pair(mesh, face));

			if(octree.isEmpty())
			{
				//tak vieme ze mozeme tento octree cely vymazat zo sceny
				m_res.coordsOctreeMap.erase(coordsOctreeMapIt);
			}
		}

		//vymaz to aj z std::map
		faceOctreeCoordsMap.erase(faceOctreeCoordsMapIt);
		if(faceOctreeCoordsMap.empty())
		{
			m_res.meshFaceOctreeCoordsMap.erase(meshFaceOctreeCoordsMapIt);
		}
	}


private:

	Octree<SceneResources::MeshFacePair>* retrieveCreatedOctree(glm::vec3 octreeVoxelBounds)
	{
		SceneResources::CoordsOctreeMap::iterator coordsOctreeMapIt = m_res.coordsOctreeMap.find(octreeVoxelBounds);

		bool isOctreeCreated = coordsOctreeMapIt != m_res.coordsOctreeMap.end();
		Octree<SceneResources::MeshFacePair>* retOctree = nullptr;

		//FIND IF THE OCTREE ON THAT VOXEL AREA EXISTS
		if (!isOctreeCreated)
		{
			//CREATE NEW OCTREE ON THAT VOXEL COORDS
			auto [octreeMinBounds, octreeMaxBounds] = SceneUtilities::calculateOctreeBounds(octreeVoxelBounds, m_voxelSize);

			//RETRIEVE NEWLY CREATED OCTREE
			retOctree = &m_res.coordsOctreeMap.emplace(
				octreeVoxelBounds, Octree<SceneResources::MeshFacePair>(octreeMinBounds, octreeMaxBounds)).first->second;
		}
		else
		{
			//RETRIEVE ALREADY EXISTING OCTREE
			retOctree = &coordsOctreeMapIt->second;
		}

		return retOctree;
	}

	glm::vec3 m_voxelSize;
};