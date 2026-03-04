#pragma once

#include "../Scene/Mesh.h"
#include "../Primitives/AABB.h"
#include "../Scene/Camera.h"
#include "../Structures/Octree.h"
#include "../Core/Window.h"
#include "Structures/PrintableMesh.h"

struct SceneResources
{
	using FaceOctreeCoordsMap = std::map<ExtendedFace*, std::vector<glm::vec3>>;
	using MeshFaceOctreeCoordsMap = std::map<Mesh*, FaceOctreeCoordsMap>;
	using MeshFacePair = std::pair<Mesh*, ExtendedFace*>;
	using CoordsOctreeMap = std::map<glm::vec3, Octree<MeshFacePair>>;

	using MatFacesMap = std::map<Material*, std::vector<ExtendedFace*>>;
	using MeshFacesMap = std::map<Mesh*, MatFacesMap>;

    using PrintableMeshMap = std::map<Mesh*, PrintableMesh*>;
    
    PrintableMeshMap printableMeshMap;
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

	Scene(float xSize, float ySize, float zSize);

	~Scene() {}
	glm::vec3 getVoxelSize();

	void addFaceIntoOctrees(Mesh* mesh, ExtendedFace* face);

	void deleteMeshFromOctrees(Mesh* mesh);

	void deleteFaceFromOctrees(Mesh* mesh, ExtendedFace* face);


private:

	Octree<SceneResources::MeshFacePair>* retrieveCreatedOctree(glm::vec3 octreeVoxelBounds);

	glm::vec3 m_voxelSize;
};
