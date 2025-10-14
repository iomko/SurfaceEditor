#pragma once
#include "../Params/OperationParams.h"

class DeleteMeshCommand : public Command<MeshParams>, public Observable
{
public:
	virtual void execute(const MeshParams& params) override
	{
		notifyObservers(params);
	}

	void undo() override
	{

	}
};
