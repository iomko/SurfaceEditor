#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"
#include "CallableIDs.h"

class MeshOutlinerAdderCallable : public Callable<MESH_OUTLINER_ADDER_CALLABLE, MeshParams, void>
{
public:
	void invoke(const MeshParams& input) override;
};
