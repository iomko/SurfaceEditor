#pragma once
#include "Callback.h"
#include "../FunctionComposer.h"
#include "../Patterns/Observer.h"
#include "../Commands/CmdProperties/CmdProperties.h"

class FetchSurfaceCallBack : public SimpleCallback<FetchSurfaceParams>, public Observable, public Observer
{
public:
	explicit FetchSurfaceCallBack(const FunctionComposer& functionComposer)
		: SimpleCallback<FetchSurfaceParams>(functionComposer)
	{}
};