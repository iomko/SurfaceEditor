#pragma once

#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"

class SelectMeshCallBack : public Callback<>, public Observer
{
public:
	virtual void execute() override;
};
