#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class DeselectFaceCommand : public Observable, public Command<DESELECT_FACE_COMMAND> {
public:
	virtual void execute() override;

	void undo() override;
};
