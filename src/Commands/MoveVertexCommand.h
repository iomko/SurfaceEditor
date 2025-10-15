#pragma once
#include "../Params/OperationParams.h"
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class MoveVertexCommand : public Command<VertexParams>, public Observable
{
public:
	virtual void execute(const VertexParams& params) override
	{
		notifyObservers(params);
	}

	void undo() override
	{
		
	}
};
