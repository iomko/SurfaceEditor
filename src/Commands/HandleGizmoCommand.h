#pragma once
#include "../Params/OperationParams.h"

class HandleGizmoCommand : public Command<GizmoParams>, public Observable
{
public:
	virtual void execute(const GizmoParams& iParams) override
	{
		notifyObservers(iParams);
	}

	void undo() override
	{

	}
};
