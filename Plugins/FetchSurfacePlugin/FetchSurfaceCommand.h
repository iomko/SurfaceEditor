#pragma once
#include "../../src/Patterns/Observer.h"
#include "../../src/Patterns/Command.h"

class FetchSurfaceCommand : public Command<OpenTopoParams>, public Observable
{
public:
	virtual void execute(const OpenTopoParams& params) override;

	void undo() override;

};
