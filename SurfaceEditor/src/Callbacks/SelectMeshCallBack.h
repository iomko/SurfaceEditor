#pragma once

class SelectMeshCallBack : public Callback<>, public Observer
{
public:
	virtual void execute() override
	{
		Camera* camera = ViewPortsHolderContext::s_camera;
		Window* window = ViewPortsHolderContext::s_window;
		Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
		std::pair<SceneResources::MeshFacePair, glm::vec3> meshFaceHitPair = SceneUtilities::retClosestHitData(camera, window, scene->m_res);

		SceneResources::MeshFacePair meshFacePair = meshFaceHitPair.first;

		Mesh* mesh = meshFacePair.first;

		ViewPortsHolderContext::s_selectionController->registerMesh(mesh);
	}
};
