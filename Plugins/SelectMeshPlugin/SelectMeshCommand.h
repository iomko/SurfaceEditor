#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"

class SelectMeshCommand : public Observable, public Command<> {
public:
	void execute() override;

	void undo() override;
};
