#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"

class DeselectMeshCallBack : public Callback<>, public Observer
{
public:
	void execute() override;
};
