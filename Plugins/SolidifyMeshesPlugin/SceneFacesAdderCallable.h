#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"

class SceneFacesAdderCallable : public Callable<FaceParams, void>
{
public:
	void invoke(const FaceParams& input) override;
};
