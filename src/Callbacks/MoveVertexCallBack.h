#pragma once
#include "../Params/OperationParams.h"
#include "Callback.h"
#include "../Patterns/Observer.h"
#include "CallbackIDs.h"

class MoveVertexCallBack : public Callback<MOVE_VERTEX_CALLBACK, VertexParams>, public Observer
{
public:
	void execute(const VertexParams& iParams) override;
};
