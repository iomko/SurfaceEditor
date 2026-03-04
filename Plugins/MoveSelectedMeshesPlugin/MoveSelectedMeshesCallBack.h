#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Params/OperationParams.h"


class MoveSelectedMeshesCallBack : public Callback<MOVE_SELECTED_MESHES_CALLBACK, MoveSelectedMeshesParams>, public Observer
{
public:

	void execute(const MoveSelectedMeshesParams& iParams) override;
};
