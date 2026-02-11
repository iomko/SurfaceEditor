#pragma once
#include "../Params/OperationParams.h"
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "CommandIDs.h"

class MoveMeshCommand : public Command<MOVE_MESH_COMMAND, MoveMeshParams>, public Observable
{
public:
	virtual void execute(const MoveMeshParams& params) override;

	void undo() override;
};
