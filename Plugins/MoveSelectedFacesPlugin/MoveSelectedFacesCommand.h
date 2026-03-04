#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class MoveSelectedFacesCommand : public Command<MOVE_SELECTED_FACES_COMMAND, MoveSelectedFacesParams>, public Observable
{
public:
	virtual void execute(const MoveSelectedFacesParams& iParams) override;

	void undo() override;
};
