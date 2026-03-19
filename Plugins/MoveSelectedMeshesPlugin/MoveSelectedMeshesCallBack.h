#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Params/OperationParams.h"


class MoveSelectedMeshesCallBack : public Callback<MoveSelectedMeshesParams>, public Observer
{
public:

	void execute(const MoveSelectedMeshesParams& iParams) override;
};
