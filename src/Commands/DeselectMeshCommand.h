#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class DeselectMeshCommand : public Observable, public Command<> {
public:
	virtual void execute() override;

	void undo() override;
};
