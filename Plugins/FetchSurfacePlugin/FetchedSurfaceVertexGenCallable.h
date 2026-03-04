#pragma once
#include "../../src/Callables/Callable.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Callables/CallableIDs.h"

class FetchedSurfaceVertexGenCallable : public Callable<FETCHED_SURFACE_VERTEX_GEN_CALLABLE, OpenTopoParams, MeshParams>
{
	void invoke(const OpenTopoParams& input, MeshParams& output) override;
};
