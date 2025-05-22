#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"
#include "../ViewPortsHolder.h"

class SceneMeshAdderCallable : public Callable<MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override
	{
		Mesh* mesh = input.m_mesh;

		Scene* scene = ViewPortsHolderContext::m_viewPortsHolder->m_scene;

		SceneRes::MeshFaceOctreeCoordsMap& meshFaceOctreeCoordsMap = scene->m_res.meshFaceOctreeCoordsMap;

		bool sceneContainsMesh = meshFaceOctreeCoordsMap.find(mesh) != scene->m_res.meshFaceOctreeCoordsMap.end();
		if (!sceneContainsMesh)
		{
			for (HalfEdgeDS::Face* face : mesh->getHalfEdgeStructure()->m_faces)
			{
				scene->addFaceIntoOctrees(mesh, face);
			}
		}
	}
};