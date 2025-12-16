#pragma once
#include "Callable.h"
#include "../Params/OperationParams.h"
#include "CallableIDs.h"

class SceneFacesAdderCallable : public Callable<SCENE_FACES_ADDER_CALLABLE, FaceParams, void>
{
public:
	void invoke(const FaceParams& input) override;
};
