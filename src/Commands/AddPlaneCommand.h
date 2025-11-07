#pragma once
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"

class AddPlaneCommand : public Command<PlaneParams>, public Observable
{
public:
	virtual void execute(const PlaneParams& params) override;

	void undo() override;
};			
