#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class DeleteSelectedFacesCommand : public Command<DELETE_SELECTED_FACES_COMMAND>, public Observable
{
public:
	virtual void execute() override;

	void undo() override;
};
