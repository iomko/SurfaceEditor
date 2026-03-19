#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"

class MoveSelectedFacesCallBack : public Callback<MoveSelectedFacesParams>, public Observer
{
public:
    MoveSelectedFacesCallBack();

	void execute(const MoveSelectedFacesParams& iParams) override;
};
