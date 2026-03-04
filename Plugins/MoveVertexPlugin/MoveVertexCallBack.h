#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Callbacks/CallbackIDs.h"

class MoveVertexCallBack : public Callback<MOVE_VERTEX_CALLBACK, VertexParams>, public Observer
{
public:
	void execute(const VertexParams& iParams) override;
};
