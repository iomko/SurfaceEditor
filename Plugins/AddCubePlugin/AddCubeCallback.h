#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Callables/FunctionComposer.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandRegistry.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/CallbackIDs.h"

class AddCubeCallback : public ComposedCallback<ADD_CUBE_CALLBACK, CubeParams>, public Observer
{
public:
	AddCubeCallback(std::unique_ptr<FunctionComposer> functionComposer);
};