#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class DeleteFaceCommand : public Command<SingleFaceParams>, public Observable
{
public:
	virtual void execute(const SingleFaceParams& params) override;

	void undo() override;
};
