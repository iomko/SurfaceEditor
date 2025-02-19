#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"
#include "CmdProperties/CmdProperties.h"

class VertexModeChangeCommand : public Observable, public Command {
public:
	virtual void execute() override {
		notifyObservers();
	}

	void undo() override {
	}

	static constexpr std::string_view getCommandName() noexcept { return "VertexModeChangeCommand"; }
};
