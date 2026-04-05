#pragma once
#include "../../src/Callables/Callable.h"
#include "../../src/Params/OperationParams.h"

class FetchedSurfaceVertexGenCallable : public Callable<OpenTopoParams, MeshParams>
{
	void invoke(const OpenTopoParams& input, MeshParams& output) override;
};
