#pragma once
#include <string_view>

#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"
#include "CmdProperties/CmdProperties.h"


class DeleteSelectedFacesCommand : public Command<>, public Observable
{
public:
	virtual void execute() override
	{
		notifyObservers();
	}

	void undo() override
	{

	}

	static constexpr std::string_view getCommandName() noexcept { return "DeleteSelectedFacesCommand"; }
};