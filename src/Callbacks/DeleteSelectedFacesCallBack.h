#pragma once
#include "../Params/OperationParams.h"
#include "Callback.h"
#include "CallbackIDs.h"
#include "../Patterns/Observer.h"

class DeleteSelectedFacesCallBack : public Callback<DELETE_SELECTED_FACES_CALLBACK>, public Observer
{
public:
    DeleteSelectedFacesCallBack();

	void execute() override;

private:
};
