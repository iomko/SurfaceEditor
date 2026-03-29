#include "DeselectFaceCallBack.h"
#include "CallbackRegister.h"
#include "../Editing/Selection/FaceSelectionManager.h"
#include "../ViewPortsController.h"
#include "../Scene/Scene.h"

static AutoRegisterCallback<DeselectFaceCallBack> autoRegisterDeselectFaceCallBack;
void DeselectFaceCallBack::execute()
{
    // Camera *camera = ViewPortsHolderContext::s_camera;
    // Window *window = ViewPortsHolderContext::s_window;
    // Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;
    // std::pair<SceneResources::MeshFacePair, glm::vec3> meshFaceHitPair = SceneUtilities::retClosestHitData(camera, window, scene->m_res);

    // SceneResources::MeshFacePair meshFacePair = meshFaceHitPair.first;

    // Mesh *mesh = meshFacePair.first;
    // ExtendedFace *face = meshFacePair.second;

    // ViewPortsHolderContext::s_selectionController->unregisterFace(mesh, face);
}