#pragma once

class AddPlaneCommand : public Command<PlaneParams>, public Observable
{
public:
	virtual void execute(const PlaneParams& params) override
	{
		notifyObservers(params);
	}

	void undo() override
	{
		
	}

	static constexpr std::string_view getCommandName() noexcept { return "AddPlaneCommand"; }
};