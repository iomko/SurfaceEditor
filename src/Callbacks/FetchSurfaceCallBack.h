#pragma once
#include "../Callbacks/Callback.h"
#include "../Callables/FunctionComposer.h"
#include "../Patterns/Observer.h"
class FetchSurfaceCallBack : public ComposedCallback<OpenTopoParams>, public Observable, public Observer
{
public:
	explicit FetchSurfaceCallBack(const FunctionComposer& functionComposer)
		: ComposedCallback<OpenTopoParams>(functionComposer)
	{}
};