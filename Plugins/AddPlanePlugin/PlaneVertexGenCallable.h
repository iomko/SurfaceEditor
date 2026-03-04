#pragma once
#include "../../src/Callables/Callable.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Callables/CallableIDs.h"

class PlaneVertexGenCallable : public Callable<PLANE_VERTEX_GEN_CALLABLE, PlaneParams, MeshParams>
{
public:
	void invoke(const PlaneParams& input, MeshParams& output) override;
};
