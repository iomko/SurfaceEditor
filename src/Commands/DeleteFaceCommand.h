#pragma once
#include "../Params/OperationParams.h"

class DeleteFaceCommand : public Command<SingleFaceParams>, public Observable
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
