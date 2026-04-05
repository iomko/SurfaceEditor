#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"

class DeleteSelectedFacesCallBack : public Callback<>, public Observer
{
public:
    DeleteSelectedFacesCallBack();

	void execute() override;

private:
};
