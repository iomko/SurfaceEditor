#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"

class SelectFaceCallBack : public Callback<>, public Observer
{
public:
	virtual void execute() override;
};