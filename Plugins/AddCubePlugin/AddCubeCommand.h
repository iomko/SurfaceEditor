#pragma once
#include "../../src/Patterns/Observer.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Params/OperationParams.h"

class AddCubeCommand : public Command<CubeParams>, public Observable
{
public:
	virtual void execute(const CubeParams& params) override;

	void undo() override;
};
