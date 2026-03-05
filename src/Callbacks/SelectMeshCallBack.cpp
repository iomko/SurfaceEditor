#include "SelectMeshCallBack.h"
#include "CallbackRegister.h"
#include "../ViewPortsController.h"

static AutoRegisterCallback<SelectMeshCallBack> reg;

void SelectMeshCallBack::execute()
{
    Camera *camera = ViewPortsHolderContext::s_camera;
    Window *window = ViewPortsHolderContext::s_window;
    Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
    std::pair<SceneResources::MeshFacePair, glm::vec3> meshFaceHitPair = SceneUtilities::retClosestHitData(camera, window, scene->m_res);

    SceneResources::MeshFacePair meshFacePair = meshFaceHitPair.first;

    Mesh *mesh = meshFacePair.first;

    ViewPortsHolderContext::s_selectionController->registerMesh(mesh);
}