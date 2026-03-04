#pragma once
#include "../../src/Patterns/Observer.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Commands/CommandIDs.h"

class FetchSurfaceCommand : public Command<FETCH_SURFACE_COMMAND, OpenTopoParams>, public Observable
{
public:
	virtual void execute(const OpenTopoParams& params) override;

	void undo() override;

};
