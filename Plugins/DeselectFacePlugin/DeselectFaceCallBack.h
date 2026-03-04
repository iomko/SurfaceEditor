#pragma once
#include "../../src/Callbacks/Callback.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Callbacks/CallbackIDs.h"

class DeselectFaceCallBack : public Callback<DESELECT_FACE_CALLBACK>, public Observer
{
public:
	virtual void execute() override;
};