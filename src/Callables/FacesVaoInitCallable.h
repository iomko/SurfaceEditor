#pragma once

#include "../Params/OperationParams.h"
#include "Callable.h"

#include "CallableIDs.h"

class FacesVaoInitCallable : public Callable<FACES_VAO_INIT_CALLABLE, FaceParams, void>
{
public:
	void invoke(const FaceParams& input) override;

};


