#pragma once

class TransformMeshCommand : public Observable, public Command<TransformMeshParams> {
public:
	virtual void execute(const TransformMeshParams& params) override {
		notifyObservers(params);
	}

	void undo() override {
	}

	static constexpr std::string_view getCommandName() noexcept { return "TransformMeshCommand"; }
};