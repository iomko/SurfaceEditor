#pragma once
#include "../Params/OperationParams.h"
#include "CallbackIDs.h"
#include "../Patterns/Observer.h"
#include "Callback.h"

class SelectionLayerCallBack : public Callback<SELECTION_LAYER_CALLBACK, SelectionLayerParams>, public Observer
{
public:
	virtual void execute(const SelectionLayerParams& params);
};
