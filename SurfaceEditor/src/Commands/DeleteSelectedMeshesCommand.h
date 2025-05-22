#pragma once

class DeleteSelectedMeshesCommand : public Command<>, public Observable
{
public:
	virtual void execute() override
	{
		notifyObservers();
	}

	void undo() override
	{

	}

	static constexpr std::string_view getCommandName() noexcept { return "DeleteSelectedMeshesCommand"; }
};