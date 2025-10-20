#pragma once
#include "Callback.h"
#include "../Patterns/Observer.h"
#include "../ViewPortsController.h"
class BoxSelectionCallBack : public Callback<BoxSelectionParams>, public Observer
{
public:
	virtual void execute(const BoxSelectionParams& params) override
	{
        glm::vec2 start_mouse_pos = params.start_mouse_pos;
        glm::vec2 end_mouse_pos = glm::vec2(Input::getMouseX(), Input::getMouseY());
		Camera* camera = ViewPortsHolderContext::s_camera;
		Window* window = ViewPortsHolderContext::s_window;
		Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
		std::vector<SceneResources::MeshFacePair> hitDataInBoxSelection = SceneUtilities::retAllHitDataInBoxSelection(camera, window, scene->m_res, start_mouse_pos, end_mouse_pos);

        for (SceneResources::MeshFacePair& meshFaceHitPair : hitDataInBoxSelection)
        {
            Mesh* mesh = meshFaceHitPair.first;
            ExtendedFace* face = meshFaceHitPair.second;
            ViewPortsHolderContext::s_selectionController->registerFace(mesh, face);
        }
	}
};