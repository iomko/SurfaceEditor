#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Callbacks/Callback.h"

class SelectionLayerCallBack : public Callback<SELECTION_LAYER_CALLBACK, SelectionLayerParams>, public Observer
{
public:
	virtual void execute(const SelectionLayerParams& params);
};
