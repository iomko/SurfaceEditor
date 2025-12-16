#include "SceneMeshAdderCallable.h"
#include "CallableRegistry.h"
#include "../ViewPortsController.h"
static AutoRegisterCallable<SceneMeshAdderCallable> reg;

void SceneMeshAdderCallable::invoke(const MeshParams &input) 
{
    Mesh *mesh = input.m_mesh;

    Scene *scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

    SceneResources::MeshFaceOctreeCoordsMap &meshFaceOctreeCoordsMap = scene->m_res.meshFaceOctreeCoordsMap;

    bool sceneContainsMesh = meshFaceOctreeCoordsMap.find(mesh) != scene->m_res.meshFaceOctreeCoordsMap.end();
    if (!sceneContainsMesh)
    {
        for (ExtendedFace *face : mesh->getHalfEdgeStructure()->m_faces)
        {
            scene->addFaceIntoOctrees(mesh, face);
        }
    }
}