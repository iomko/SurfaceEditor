#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Callbacks/CallbackIDs.h"

class DeselectMeshCallBack : public Callback<DESELECT_MESH_CALLBACK>, public Observer
{
public:
	void execute() override;
};
