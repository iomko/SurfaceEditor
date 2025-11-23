#pragma once
#include "../Callbacks/Callback.h"
#include "../Callables/FunctionComposer.h"
#include "../Patterns/Observer.h"
#include "../Params/OperationParams.h"
#include "CallbackIDs.h"

class FetchSurfaceCallBack : public ComposedCallback<FETCH_SURFACE_CALLBACK ,OpenTopoParams>, public Observable, public Observer
{
public:
	explicit FetchSurfaceCallBack(std::unique_ptr<FunctionComposer> functionComposer);
};