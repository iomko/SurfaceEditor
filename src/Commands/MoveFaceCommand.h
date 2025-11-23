#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "CommandIDs.h"

class MoveFaceCommand : public Command<MOVE_FACE_COMMAND ,SingleFaceParams>, public Observable
{
public:
	virtual void execute(const SingleFaceParams& params) override;

	void undo() override;
};
