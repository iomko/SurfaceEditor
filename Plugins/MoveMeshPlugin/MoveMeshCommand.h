#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class MoveMeshCommand : public Command<MOVE_MESH_COMMAND, MoveMeshParams>, public Observable
{
public:
	virtual void execute(const MoveMeshParams& params) override;

	void undo() override;
};
