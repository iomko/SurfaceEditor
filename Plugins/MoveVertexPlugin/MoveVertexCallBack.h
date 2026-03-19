#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"

class MoveVertexCallBack : public Callback<VertexParams>, public Observer
{
public:
	void execute(const VertexParams& iParams) override;
};
