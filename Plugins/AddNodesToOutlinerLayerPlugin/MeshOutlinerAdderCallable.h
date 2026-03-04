#pragma once
#include "../../src/Callables/Callable.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Callables/CallableIDs.h"

class MeshOutlinerAdderCallable : public Callable<MESH_OUTLINER_ADDER_CALLABLE, MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override;
};
