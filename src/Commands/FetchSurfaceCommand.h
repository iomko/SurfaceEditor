#pragma once
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"

class FetchSurfaceCommand : public Command<OpenTopoParams>, public Observable
{
public:
	virtual void execute(const OpenTopoParams& params) override;

	void undo() override;

};
