#pragma once

class AddPlaneCallback : public ComposedCallback<PlaneParams>, public Observer
{
public:
	AddPlaneCallback(const FunctionComposer& functionComposer)
		: ComposedCallback(functionComposer)
	{}
};