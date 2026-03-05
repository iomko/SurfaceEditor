#pragma once
#include "Callback.h"
#include "../Patterns/Observer.h"
#include "CallbackIDs.h"
#include "../Params/OperationParams.h"


class MoveSelectedMeshesCallBack : public Callback<MOVE_SELECTED_MESHES_CALLBACK, MoveSelectedMeshesParams>, public Observer
{
public:

	void execute(const MoveSelectedMeshesParams& iParams) override;
};
