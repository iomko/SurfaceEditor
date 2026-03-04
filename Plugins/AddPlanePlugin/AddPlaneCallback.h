#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Callables/FunctionComposer.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Params/OperationParams.h"
#include "../../src/Callbacks/CallbackIDs.h"

class AddPlaneCallback : public ComposedCallback<ADD_PLANE_CALLBACK, PlaneParams>, public Observer
{
public:
	AddPlaneCallback(std::unique_ptr<FunctionComposer> functionComposer);
};