#pragma once
#include "Callable.h"
#include "../Commands/CmdProperties/CmdProperties.h"
#include "../ViewPortsHolder.h"


class AddMeshIntoSceneCallable : public Callable<MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override
	{
		Mesh* mesh = input.m_mesh;

		Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;
		Scene::MeshFaceOctreeCoordsMap& meshFaceOctreeCoordsMap = scene->meshFaceOctreeCoordsMap;

		bool sceneContainsMesh = meshFaceOctreeCoordsMap.find(mesh) != scene->meshFaceOctreeCoordsMap.end();
		if (!sceneContainsMesh)
		{
			//MESH IS NOT CONTAINED INSIDE THE SCENE
			//THEN WE KNOW WE WANT TO ADD THE FACES INTO THE SCENE
			addMeshFacesIntoOctree(input);
		}
	}

private:

	Octree<Scene::MeshFacePair>* retrieveCreatedOctree(glm::vec3 octreeVoxelBounds)
	{
		Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;

		Scene::CoordsOctreeMap& coordsOctreeMap = scene->coordsOctreeMap;
		Scene::CoordsOctreeMap::iterator coordsOctreeMapIt = coordsOctreeMap.find(octreeVoxelBounds);

		bool isOctreeCreated = coordsOctreeMapIt != coordsOctreeMap.end();
		Octree<Scene::MeshFacePair>* retOctree = nullptr;

		//FIND IF THE OCTREE ON THAT VOXEL AREA EXISTS
		if (!isOctreeCreated)
		{
			//CREATE NEW OCTREE ON THAT VOXEL COORDS
			auto [octreeMinBounds, octreeMaxBounds] = SceneUtilities::calculateOctreeBounds(octreeVoxelBounds, scene->getVoxelSize());

			//RETRIEVE NEWLY CREATED OCTREE
			retOctree = &scene->coordsOctreeMap.emplace(
				octreeVoxelBounds, Octree<Scene::MeshFacePair>(octreeMinBounds, octreeMaxBounds)).first->second;
		}
		else
		{
			//RETRIEVE ALREADY EXISTING OCTREE
			retOctree = &coordsOctreeMapIt->second;
		}

		return retOctree;
	}

	void addMeshFacesIntoOctree(const MeshParams& meshParams)
	{
		Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;
		Mesh* mesh = meshParams.m_mesh;

		Scene::MeshFaceOctreeCoordsMap& meshFaceOctreeCoordsMap = scene->meshFaceOctreeCoordsMap;
		Scene::FaceOctreeCoordsMap& faceOctreeCoordsMap = meshFaceOctreeCoordsMap[mesh];

		//GO THROUGH ALL FACES TO ADD THEM INTO THE CORRESPONDING MAPS FOR THE SPECIFIED MESH
		for (auto faceIter = mesh->m_halfEdgeStructure->faceIterBegin(); faceIter != mesh->m_halfEdgeStructure->faceIterEnd(); ++faceIter)
		{
			//COMPUTE BOUNDING BOX OF THE FACE
			std::vector<HalfEdgeDS::Vertex> faceVerts;
			mesh->m_halfEdgeStructure->getVerticesFromFace(*faceIter, faceVerts);
			AABBBoundingRegion faceBounds(
				faceVerts.begin(),
				faceVerts.end(),
				[](HalfEdgeDS::Vertex& p) { return p.m_position.x; },
				[](HalfEdgeDS::Vertex& p) { return p.m_position.y; },
				[](HalfEdgeDS::Vertex& p) { return p.m_position.z; }
			);

			glm::vec3 sceneVoxelMinCoords = SceneUtilities::getVoxelIndex(faceBounds.getMin(), scene->getVoxelSize());
			glm::vec3 sceneVoxelMaxCoords = SceneUtilities::getVoxelIndex(faceBounds.getMax(), scene->getVoxelSize());

			for (int x = sceneVoxelMinCoords.x; x <= sceneVoxelMaxCoords.x; ++x)
			{
				for (int y = sceneVoxelMinCoords.y; y <= sceneVoxelMaxCoords.y; ++y)
				{
					for (int z = sceneVoxelMinCoords.z; z <= sceneVoxelMaxCoords.z; ++z)
					{
						glm::vec3 currentVoxelBounds = { x,y,z };

						Octree<Scene::MeshFacePair>* insertionOctree = retrieveCreatedOctree(currentVoxelBounds);

						insertionOctree->addDataToOctree(Scene::MeshFacePair(mesh, *faceIter), faceBounds);

						//insertionOctree->addDataToOctree(Scene::MeshFacePair(mesh, &(*faceIter)), faceBounds);

						//ADD FACE INTO THE MESH_FACE_OCTREE_COORDS_MAP
						faceOctreeCoordsMap[*faceIter].push_back(currentVoxelBounds);
						//faceOctreeCoordsMap[&(*faceIter)].push_back(currentVoxelBounds);
					}
				}

			}

		}
	}
};