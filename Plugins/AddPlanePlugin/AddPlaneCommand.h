#pragma once
#include "../../src/Patterns/Observer.h"
#include "../../src/Patterns/Command.h"

class AddPlaneCommand : public Command<PlaneParams>, public Observable
{
public:
	virtual void execute(const PlaneParams& params) override;

	void undo() override;
};			
