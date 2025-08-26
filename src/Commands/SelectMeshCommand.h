#pragma once

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