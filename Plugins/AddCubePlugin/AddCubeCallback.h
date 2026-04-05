#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Callables/FunctionComposer.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Params/OperationParams.h"

class AddCubeCallback : public ComposedCallback<CubeParams>, public Observer
{
public:
	AddCubeCallback(std::unique_ptr<FunctionComposer> functionComposer);
};