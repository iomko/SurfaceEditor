#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"
#include "CallableIDs.h"

class SceneMeshAdderCallable : public Callable<SCENE_MESH_ADDER_CALLABLE, MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override;
};