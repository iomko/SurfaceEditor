#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"
#include "CallableIDs.h"

class PlaneVertexGenCallable : public Callable<PLANE_VERTEX_GEN_CALLABLE, PlaneParams, MeshParams>
{
public:
	void invoke(const PlaneParams& input, MeshParams& output) override;
};
