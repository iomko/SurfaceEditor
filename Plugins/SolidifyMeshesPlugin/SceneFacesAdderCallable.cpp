#include "SceneFacesAdderCallable.h"
#include "../../src/Callables/CallableRegistry.h"
#include "../../src/ViewPortsController.h"

static AutoRegisterCallable<SceneFacesAdderCallable> reg("SCENE_FACES_ADDER_CALLABLE");

void SceneFacesAdderCallable::invoke(const FaceParams &input)
{
    Mesh *mesh = input.mesh;
    std::vector<ExtendedFace *> &faces = *(input.faces);

    Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

    for (ExtendedFace *face : faces)
    {
        scene->addFace(mesh, face);
    }
}