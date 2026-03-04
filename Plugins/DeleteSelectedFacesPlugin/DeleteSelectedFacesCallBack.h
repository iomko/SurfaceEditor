#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/Callback.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Patterns/Observer.h"

class DeleteSelectedFacesCallBack : public Callback<DELETE_SELECTED_FACES_CALLBACK>, public Observer
{
public:
    DeleteSelectedFacesCallBack();

	void execute() override;

private:
};
