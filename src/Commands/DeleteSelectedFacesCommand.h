#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class DeleteSelectedFacesCommand : public Command<>, public Observable
{
public:
	virtual void execute() override;

	void undo() override;
};
