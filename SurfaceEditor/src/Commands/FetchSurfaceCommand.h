#pragma once

class FetchSurfaceCommand : public Command<OpenTopoParams>, public Observable
{
public:
	virtual void execute(const OpenTopoParams& params) override
	{
		notifyObservers(params);
	}

	void undo() override
	{
	}

	static constexpr std::string_view getCommandName() noexcept { return "FetchSurfaceCommand"; }
};