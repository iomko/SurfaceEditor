#pragma once
#include "Callback.h"
#include "../Patterns/Observer.h"
#include "CallbackIDs.h"

class DeselectMeshCallBack : public Callback<DESELECT_MESH_CALLBACK>, public Observer
{
public:
	void execute() override;
};
