#pragma once
#include "../Callbacks/Callback.h"
#include "../Callables/FunctionComposer.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"

class AddCubeCallback : public ComposedCallback<CubeParams>, public Observer
{
public:
	AddCubeCallback(const FunctionComposer& functionComposer)
		: ComposedCallback(functionComposer)
	{
		
	}
};