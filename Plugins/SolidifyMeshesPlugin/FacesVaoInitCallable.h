#pragma once

#include "../../src/Params/OperationParams.h"
#include "../../src/Callables/Callable.h"


class FacesVaoInitCallable : public Callable<FaceParams, void>
{
public:
	void invoke(const FaceParams& input) override;

};


