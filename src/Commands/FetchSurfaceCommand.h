#pragma once
#include "../Patterns/Observer.h"

class FetchSurfaceCommand : public Command<OpenTopoParams>, public Observable
{
public:
	virtual void execute(const OpenTopoParams& params) override
	{
		notifyObservers(params);
	}

	void undo() override
	{
	}

};
