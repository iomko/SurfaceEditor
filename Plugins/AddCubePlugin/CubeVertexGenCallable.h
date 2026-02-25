#pragma once
#include "../../src/Callables/Callable.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Callables/CallableIDs.h"

class CubeVertexGenCallable : public Callable<CUBE_VERTEX_GEN_CALLABLE, CubeParams, MeshParams>
{
public:
	void invoke(const CubeParams& input, MeshParams& output) override;
};
