#pragma once
#include "Patterns/Command.h"
#include "Patterns/Observer.h"

class SolidifyMeshesCommand : public Command<>, public Observable
{
public:
	virtual void execute() override
	{
		notifyObservers();
	}

	void undo() override
	{
		
	}

	static constexpr std::string_view getCommandName() noexcept { return "SolidifyMeshesCommand"; }
};
