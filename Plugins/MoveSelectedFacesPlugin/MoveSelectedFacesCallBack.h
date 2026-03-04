#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Callbacks/CallbackIDs.h"

class MoveSelectedFacesCallBack : public Callback<MOVE_SELECTED_FACES_CALLBACK, MoveSelectedFacesParams>, public Observer
{
public:
    MoveSelectedFacesCallBack();

	void execute(const MoveSelectedFacesParams& iParams) override;
};
