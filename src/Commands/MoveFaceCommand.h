#pragma once
#include "../Params/OperationParams.h"
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class MoveFaceCommand : public Command<SingleFaceParams>, public Observable
{
public:
	virtual void execute(const SingleFaceParams& params) override
	{
		notifyObservers(params);
	}

	void undo() override
	{
		
	}
};
