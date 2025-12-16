#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"

#include "CallableIDs.h"

class MeshVaoInitCallable : public Callable<MESH_VAO_INIT_CALLABLE, MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override;
};
