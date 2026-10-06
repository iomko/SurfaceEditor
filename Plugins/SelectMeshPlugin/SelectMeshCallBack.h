#pragma once

#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"

class SelectMeshCallBack : public Callback<SelectionToolParams>, public Observer
{
public:
	virtual void execute(const SelectionToolParams& iParams) override;
};
