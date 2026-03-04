#pragma once

#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Callbacks/CallbackIDs.h"

class SelectMeshCallBack : public Callback<SELECT_MESH_CALLBACK>, public Observer
{
public:
	virtual void execute() override;
};
