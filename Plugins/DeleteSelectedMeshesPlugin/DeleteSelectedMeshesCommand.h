#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"

class DeleteSelectedMeshesCommand : public Command<>, public Observable
{
public:
	virtual void execute() override;

	void undo() override;
};
