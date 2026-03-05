#pragma once

#include "../Params/OperationParams.h"
#include "Callable.h"
#include "CallableIDs.h"

class EdgesVaoInitCallable : public Callable<EDGES_VAO_INIT_CALLABLE, EdgeParams, void>
{
public:
	void invoke(const EdgeParams& input) override;
};



