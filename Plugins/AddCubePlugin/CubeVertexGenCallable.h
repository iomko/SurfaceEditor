#pragma once
#include "../../src/Callables/Callable.h"
#include "../../src/Params/OperationParams.h"

class CubeVertexGenCallable : public Callable<CubeParams, MeshParams>
{
public:
	void invoke(const CubeParams& input, MeshParams& output) override;
};
