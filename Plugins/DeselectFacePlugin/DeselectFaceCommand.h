#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"

class DeselectFaceCommand : public Observable, public Command<> {
public:
	virtual void execute() override;

	void undo() override;
};
