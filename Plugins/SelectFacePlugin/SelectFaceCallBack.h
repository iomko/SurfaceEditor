#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Callbacks/CallbackIDs.h"
#include "../../src/Patterns/Observer.h"

class SelectFaceCallBack : public Callback<SELECT_FACE_CALLBACK>, public Observer
{
public:
	virtual void execute() override;
};