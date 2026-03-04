#pragma once
#include "../../src/Callables/Callable.h"
#include "../../src/Params/OperationParams.h"

#include "../../src/Callables/CallableIDs.h"

class MeshVaoInitCallable : public Callable<MESH_VAO_INIT_CALLABLE, MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override;
};
