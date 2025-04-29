#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "../ViewPortsHolder.h"
#include "../Commands/CmdProperties/CmdProperties.h"

class SelectMeshCommand : public Observable, public Command<> {
public:
	void execute() override
	{
		notifyObservers();
	}

	void undo() override {
	}

	static constexpr std::string_view getCommandName() noexcept { return "SelectMeshCommand"; }
};