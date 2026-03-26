#include "SceneFacesAdderCallable.h"
#include "CallableRegistry.h"
#include "../ViewPortsController.h"

static AutoRegisterCallable<SceneFacesAdderCallable> reg("SCENE_FACES_ADDER_CALLABLE");

void SceneFacesAdderCallable::invoke(const FaceParams &input)
{
    Mesh *mesh = input.mesh;
    std::vector<ExtendedFace *> &faces = *(input.faces);

    Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

    SceneResources::MeshFaceOctreeCoordsMap &meshFaceOctreeCoordsMap = scene->m_res.meshFaceOctreeCoordsMap;

    for (ExtendedFace *face : faces)
    {
        scene->addFaceIntoOctrees(mesh, face);
    }
}