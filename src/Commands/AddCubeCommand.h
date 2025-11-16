#pragma once
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"

class AddCubeCommand : public Command<ADD_CUBE_COMMAND, CubeParams>, public Observable
{
public:
	virtual void execute(const CubeParams& params) override;

	void undo() override;
};
