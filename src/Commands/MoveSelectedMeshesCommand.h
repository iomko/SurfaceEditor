#pragma once
#include "../Params/OperationParams.h"

class MoveSelectedMeshesCommand : public Command<MoveSelectedMeshesParams>, public Observable
{
public:
	virtual void execute(const MoveSelectedMeshesParams& iParams) override
	{
		notifyObservers(iParams);
	}

	void undo() override
	{

	}
};
