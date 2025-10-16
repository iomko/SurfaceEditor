#pragma once
#include "../Params/OperationParams.h"

class MoveSelectedFacesCommand : public Command<MoveSelectedFacesParams>, public Observable
{
public:
	virtual void execute(const MoveSelectedFacesParams& iParams) override
	{
		notifyObservers(iParams);
	}

	void undo() override
	{

	}
};
