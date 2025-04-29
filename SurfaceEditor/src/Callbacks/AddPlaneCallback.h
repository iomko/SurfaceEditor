#pragma once
#include "Callback.h"
#include "../Commands/CommandRegistry.h"
#include "../Callables/GenPlaneVertexDataCallable.h"
#include "../Callables/AddMeshIntoSceneCallable.h"

#include "../ViewPortsHolder.h"
#include "../Callables/InitMeshVaoDataCallable.h"
#include "../Commands/CmdProperties/CmdProperties.h"

#include <random>

class AddPlaneCallback : public SimpleCallback<AddPlaneParams>, public Observable, public Observer
{
public:
	explicit AddPlaneCallback(const FunctionComposer& functionComposer)
		: SimpleCallback<AddPlaneParams>(functionComposer)
	{}
};