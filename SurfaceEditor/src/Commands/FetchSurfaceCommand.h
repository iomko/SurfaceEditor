#pragma once
#include <string_view>
#include <vector>
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"
#include "CmdProperties/CmdProperties.h"


class FetchSurfaceCommand : public Command<FetchSurfaceParams>, public Observable
{
public:
	virtual void execute(const FetchSurfaceParams& params) override
	{
		notifyObservers(params);
	}

	void undo() override
	{
	}

	static constexpr std::string_view getCommandName() noexcept { return "FetchSurfaceCommand"; }
};