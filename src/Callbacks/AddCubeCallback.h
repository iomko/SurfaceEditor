#pragma once

class AddCubeCallback : public ComposedCallback<CubeParams>, public Observer
{
public:
	AddCubeCallback(const FunctionComposer& functionComposer)
		: ComposedCallback(functionComposer)
	{}
};