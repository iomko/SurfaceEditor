#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class SelectFaceCommand : public Observable, public Command<SELECT_FACE_COMMAND> {
public:
	virtual void execute() override;

	void undo() override;

};
