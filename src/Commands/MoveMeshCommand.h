#pragma once
#include "../Params/OperationParams.h"
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class MoveMeshCommand : public Command<MoveMeshParams>, public Observable
{
public:
	virtual void execute(const MoveMeshParams& params) override
	{
		notifyObservers(params);
	}

	void undo() override
	{
		
	}
};
