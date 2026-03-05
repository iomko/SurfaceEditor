#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"
#include "CallableIDs.h"

class FetchedSurfaceVertexGenCallable : public Callable<FETCHED_SURFACE_VERTEX_GEN_CALLABLE, OpenTopoParams, MeshParams>
{
	void invoke(const OpenTopoParams& input, MeshParams& output) override;
};
