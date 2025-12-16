#pragma once

#include "../Params/OperationParams.h"
#include "Callable.h"

#include "CallableIDs.h"

class FaceVaoInitCallable : public Callable<FACE_VAO_INIT_CALLABLE, SingleFaceParams, void>
{
public:
	void invoke(const SingleFaceParams& input) override;

};


