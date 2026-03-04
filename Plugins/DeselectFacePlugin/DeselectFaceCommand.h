#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class DeselectFaceCommand : public Observable, public Command<DESELECT_FACE_COMMAND> {
public:
	virtual void execute() override;

	void undo() override;
};
