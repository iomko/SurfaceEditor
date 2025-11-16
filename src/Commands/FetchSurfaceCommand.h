#pragma once
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"

class FetchSurfaceCommand : public Command<FETCH_SURFACE_COMMAND, OpenTopoParams>, public Observable
{
public:
	virtual void execute(const OpenTopoParams& params) override;

	void undo() override;

};
