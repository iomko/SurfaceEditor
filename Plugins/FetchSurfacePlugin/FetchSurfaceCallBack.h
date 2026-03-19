#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Callables/FunctionComposer.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Params/OperationParams.h"

class FetchSurfaceCallBack : public ComposedCallback<OpenTopoParams>, public Observable, public Observer
{
public:
	explicit FetchSurfaceCallBack(std::unique_ptr<FunctionComposer> functionComposer);
};