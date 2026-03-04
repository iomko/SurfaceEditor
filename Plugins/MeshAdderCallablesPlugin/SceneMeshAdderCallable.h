#pragma once
#include "../../src/Callables/Callable.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Callables/CallableIDs.h"

class SceneMeshAdderCallable : public Callable<SCENE_MESH_ADDER_CALLABLE, MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override;
};