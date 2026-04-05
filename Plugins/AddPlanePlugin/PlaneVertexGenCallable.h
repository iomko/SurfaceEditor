#pragma once
#include "../../src/Callables/Callable.h"
#include "../../src/Params/OperationParams.h"

class PlaneVertexGenCallable : public Callable<PlaneParams, MeshParams>
{
public:
	void invoke(const PlaneParams& input, MeshParams& output) override;
};
