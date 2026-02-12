#pragma once
#include "Callback.h"
#include "../Patterns/Observer.h"
#include "CallbackIDs.h"

class DeselectFaceCallBack : public Callback<DESELECT_FACE_CALLBACK>, public Observer
{
public:
	virtual void execute() override;
};