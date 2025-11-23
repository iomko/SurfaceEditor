#pragma once

#include "Callback.h"
#include "../Params/OperationParams.h"
#include "../Patterns/Observer.h"
#include "CallbackIDs.h"

class SelectionLayerCallBack : public Callback<SELECTION_LAYER_CALLBACK, SelectionLayerParams>, public Observer
{
public:
	virtual void execute(const SelectionLayerParams& params);
};
