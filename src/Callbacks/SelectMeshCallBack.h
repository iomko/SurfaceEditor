#pragma once

#include "Callback.h"
#include "../Patterns/Observer.h"
#include "CallbackIDs.h"

class SelectMeshCallBack : public Callback<SELECT_MESH_CALLBACK, SelectionToolParams>, public Observer
{
public:
	virtual void execute(const SelectionToolParams& iParams) override;
};
