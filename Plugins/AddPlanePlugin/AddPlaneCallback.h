#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Callables/FunctionComposer.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Params/OperationParams.h"

class AddPlaneCallback : public ComposedCallback<PlaneParams>, public Observer
{
public:
	AddPlaneCallback(std::unique_ptr<FunctionComposer> functionComposer);
};