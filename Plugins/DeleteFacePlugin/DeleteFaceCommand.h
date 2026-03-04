#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class DeleteFaceCommand : public Command<DELETE_FACE_COMMAND, SingleFaceParams>, public Observable
{
public:
	virtual void execute(const SingleFaceParams& params) override;

	void undo() override;
};
