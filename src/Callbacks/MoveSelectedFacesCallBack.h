#pragma once
#include "../Params/OperationParams.h"
#include "Callback.h"
#include "../Patterns/Observer.h"
#include "CallbackIDs.h"

class MoveSelectedFacesCallBack : public Callback<MOVE_SELECTED_FACE_CALLBACK, MoveSelectedFacesParams>, public Observer
{
public:
    MoveSelectedFacesCallBack();

	void execute(const MoveSelectedFacesParams& iParams) override;
};
