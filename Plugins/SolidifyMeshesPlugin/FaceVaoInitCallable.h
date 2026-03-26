#pragma once

#include "../../src/Params/OperationParams.h"
#include "../../src/Callables/Callable.h"


class FaceVaoInitCallable : public Callable<SingleFaceParams, void>
{
public:
	void invoke(const SingleFaceParams& input) override;

};


