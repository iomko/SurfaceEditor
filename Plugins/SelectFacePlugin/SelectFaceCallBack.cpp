#include "SelectFaceCallBack.h"
#include "../../src/Callbacks/CallbackRegister.h"
#include "../../src/ViewPortsController.h"

static AutoRegisterCallback<SelectFaceCallBack> reg("SELECT_FACE_CALLBACK");

void SelectFaceCallBack::execute()
{
    Camera *camera = ViewPortsHolderContext::s_camera;
    Window *window = ViewPortsHolderContext::s_window;
    Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
    std::pair<SceneResources::MeshFacePair, glm::vec3> meshFaceHitPair = SceneUtilities::retClosestHitData(camera, window, scene->m_res);

    SceneResources::MeshFacePair meshFacePair = meshFaceHitPair.first;

    Mesh *mesh = meshFacePair.first;
    ExtendedFace *face = meshFacePair.second;

    ViewPortsHolderContext::s_selectionController->registerFace(mesh, face);
}