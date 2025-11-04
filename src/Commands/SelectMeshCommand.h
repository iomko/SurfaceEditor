#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class SelectMeshCommand : public Observable, public Command<> {
public:
	void execute() override;

	void undo() override;
};
