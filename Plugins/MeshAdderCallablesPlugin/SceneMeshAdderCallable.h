#pragma once
#include "../../src/Callables/Callable.h"
#include "../../src/Params/OperationParams.h"

class SceneMeshAdderCallable : public Callable<MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override;
};