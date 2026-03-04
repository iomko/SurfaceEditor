#pragma once
#include "../../src/Patterns/Observer.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Commands/CommandIDs.h"

class AddPlaneCommand : public Command<ADD_PLANE_COMMAND, PlaneParams>, public Observable
{
public:
	virtual void execute(const PlaneParams& params) override;

	void undo() override;
};			
