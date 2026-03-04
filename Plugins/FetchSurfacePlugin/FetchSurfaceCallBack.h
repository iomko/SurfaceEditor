#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Callables/FunctionComposer.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/CallbackIDs.h"

class FetchSurfaceCallBack : public ComposedCallback<FETCH_SURFACE_CALLBACK ,OpenTopoParams>, public Observable, public Observer
{
public:
	explicit FetchSurfaceCallBack(std::unique_ptr<FunctionComposer> functionComposer);
};