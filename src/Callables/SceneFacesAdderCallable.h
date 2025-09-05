#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"
#include "../ViewPortsController.h"

class SceneFacesAdderCallable : public Callable<FaceParams, void>
{
public:
	void invoke(const FaceParams& input) override
    {
        Mesh* mesh = input.mesh;
        std::vector<ExtendedFace*>& faces = *(input.faces);

		Scene* scene = ViewPortsHolderContext::s_viewPortsController->m_scene;

		SceneResources::MeshFaceOctreeCoordsMap& meshFaceOctreeCoordsMap = scene->m_res.meshFaceOctreeCoordsMap;

        for (ExtendedFace* face : faces)
        {
            scene->addFaceIntoOctrees(mesh, face);
        }
	}
};
