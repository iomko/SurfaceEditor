#pragma once

#include "../../src/Params/OperationParams.h"
#include "../../src/Callables/Callable.h"

class EdgesVaoInitCallable : public Callable<EdgeParams, void>
{
public:
	void invoke(const EdgeParams& input) override;
};



