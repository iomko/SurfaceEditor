#pragma once
#include "../Params/OperationParams.h"

class MoveSelectedFacesCommand : public Command<>, public Observable
{
public:
	virtual void execute() override
	{
		notifyObservers();
	}

	void undo() override
	{

	}
};
