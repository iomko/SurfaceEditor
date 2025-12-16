#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"
#include "CallableIDs.h"

class CubeVertexGenCallable : public Callable<CUBE_VERTEX_GEN_CALLABLE, CubeParams, MeshParams>
{
public:
	void invoke(const CubeParams& input, MeshParams& output) override;
};
