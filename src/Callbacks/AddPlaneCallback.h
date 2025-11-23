#pragma once
#include "Callback.h"
#include "../Callables/FunctionComposer.h"
#include "../Patterns/Observer.h"
#include "../Params/OperationParams.h"
#include "CallbackIDs.h"


class AddPlaneCallback : public ComposedCallback<ADD_PLANE_CALLBACK, PlaneParams>, public Observer
{
public:
	AddPlaneCallback(FunctionComposer* functionComposer);
};