#pragma once
#include "Callback.h"
#include "../Patterns/Observer.h"
#include "CallbackIDs.h"

class DeselectMeshCallBack : public Callback<DESELECT_MESH_CALLBACK, SelectionToolParams>, public Observer
{
public:
	void execute(const SelectionToolParams& iParams) override;
};
