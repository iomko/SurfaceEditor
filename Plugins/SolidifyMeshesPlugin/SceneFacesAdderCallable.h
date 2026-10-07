#pragma once
#include "../../src/Callables/Callable.h"
#include "../../src/Params/OperationParams.h"

class SceneFacesAdderCallable : public Callable<FaceParams, void>
{
public:
	void invoke(const FaceParams& input) override;
};
