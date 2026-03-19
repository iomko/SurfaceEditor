#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Callbacks/Callback.h"

class SelectionLayerCallBack : public Callback<SelectionLayerParams>, public Observer
{
public:
	virtual void execute(const SelectionLayerParams& params);
};
