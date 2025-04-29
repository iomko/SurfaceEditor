#pragma once
#include <vector>
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"
#include "CmdProperties/CmdProperties.h"


class AddPlaneCommand : public Command<AddPlaneParams>, public Observable
{
public:
	virtual void execute(const AddPlaneParams& params) override
	{
		notifyObservers(params);
	}

	void undo() override
	{
		
	}

	static constexpr std::string_view getCommandName() noexcept { return "AddPlaneCommand"; }
};