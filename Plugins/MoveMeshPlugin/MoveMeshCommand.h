#pragma once
#include "../../src/Params/OperationParams.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"

class MoveMeshCommand : public Command<MoveMeshParams>, public Observable
{
public:
	virtual void execute(const MoveMeshParams& params) override;

	void undo() override;
};
