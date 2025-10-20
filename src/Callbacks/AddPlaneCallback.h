#pragma once
#include "../Callbacks/Callback.h"
#include "../Callables/FunctionComposer.h"
#include "../Patterns/Observer.h"

class AddPlaneCallback : public ComposedCallback<PlaneParams>, public Observer
{
public:
	AddPlaneCallback(const FunctionComposer& functionComposer)
		: ComposedCallback(functionComposer)
	{}
};