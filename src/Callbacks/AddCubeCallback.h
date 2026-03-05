#pragma once
#include "../Callbacks/Callback.h"
#include "../Callables/FunctionComposer.h"
#include "../Patterns/Observer.h"
#include "../Commands/CommandRegistry.h"
#include "../Params/OperationParams.h"
#include "CallbackIDs.h"

class AddCubeCallback : public ComposedCallback<ADD_CUBE_CALLBACK, CubeParams>, public Observer
{
public:
	AddCubeCallback(std::unique_ptr<FunctionComposer> functionComposer);
};