#pragma once
#include "Callback.h"
#include "CallbackIDs.h"
#include "../Patterns/Observer.h"

class SelectFaceCallBack : public Callback<SELECT_FACE_CALLBACK>, public Observer
{
public:
	virtual void execute() override;
};