#include "SceneMeshAdderCallable.h"
#include "../../src/Callables/CallableRegistry.h"
#include "../../src/ViewPortsController.h"
#include "../../src/Core/Application.h"

#include "../../src/Core/Profiler.h"

static AutoRegisterCallable<SceneMeshAdderCallable> reg("SCENE_MESH_ADDER_CALLABLE");

void SceneMeshAdderCallable::invoke(const MeshParams &input) 
{
    SE_PROFILE_SCOPE_N("SceneMeshAdderCallable:invoke");
    Mesh *mesh = input.m_mesh;
    ViewPortsController* controller = ViewPortsHolderContext::s_viewPortsController.get();

    controller->m_scene->addMesh(mesh);

    ModelObject* object = input.m_object;
    if (object == nullptr)
    {
        object = Application::getInstance().getModelObjectRepository().createObject(input.m_name);
    }
    object->addMesh(mesh);
    mesh->m_parentObject = object;
}