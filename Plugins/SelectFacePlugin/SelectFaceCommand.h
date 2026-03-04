#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class SelectFaceCommand : public Observable, public Command<SELECT_FACE_COMMAND> {
public:
	virtual void execute() override;

	void undo() override;

};
