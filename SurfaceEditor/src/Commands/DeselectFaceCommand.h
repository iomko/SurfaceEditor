#pragma once

class DeselectFaceCommand : public Observable, public Command<> {
public:
	virtual void execute() override {
		notifyObservers();
	}

	void undo() override {
	}

	static constexpr std::string_view getCommandName() noexcept { return "DeselectFaceCommand"; }
};