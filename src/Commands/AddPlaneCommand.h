#pragma once
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"
#include "CommandIDs.h"

class AddPlaneCommand : public Command<ADD_PLANE_COMMAND, PlaneParams>, public Observable
{
public:
	virtual void execute(const PlaneParams& params) override;

	void undo() override;
};			
