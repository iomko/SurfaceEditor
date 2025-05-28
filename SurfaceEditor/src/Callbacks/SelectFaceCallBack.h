#pragma once

class SelectFaceCallBack : public Callback<>, public Observer
{
public:
	virtual void execute() override
	{
		Camera* camera = ViewPortsHolderContext::s_camera;
		Window* window = ViewPortsHolderContext::s_window;
		Scene* scene = ViewPortsHolderContext::s_viewPortsHolder->m_scene;
		std::pair<SceneResources::MeshFacePair, glm::vec3> meshFaceHitPair = SceneUtilities::retClosestHitData(camera, window, scene->m_res);

		SceneResources::MeshFacePair meshFacePair = meshFaceHitPair.first;

		Mesh* mesh = meshFacePair.first;
		HalfEdgeDS::Face* face = meshFacePair.second;

		ViewPortsHolderContext::s_selectionController->registerFace(mesh, face);
	}
};