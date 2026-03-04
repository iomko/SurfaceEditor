#pragma once
#include "../../src/Patterns/Observer.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Commands/CommandIDs.h"
#include "../../src/Params/OperationParams.h"

class AddCubeCommand : public Command<ADD_CUBE_COMMAND, CubeParams>, public Observable
{
public:
	virtual void execute(const CubeParams& params) override;

	void undo() override;
};
