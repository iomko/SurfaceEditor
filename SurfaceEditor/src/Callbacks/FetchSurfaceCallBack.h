#pragma once

class FetchSurfaceCallBack : public ComposedCallback<OpenTopoParams>, public Observable, public Observer
{
public:
	explicit FetchSurfaceCallBack(const FunctionComposer& functionComposer)
		: ComposedCallback<OpenTopoParams>(functionComposer)
	{}
};