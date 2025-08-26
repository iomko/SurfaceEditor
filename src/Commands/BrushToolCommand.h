#pragma once

class BrushToolCommand : public Command<BrushToolParams, OctreeNodeDataParams>, public Observable
{
public:
	virtual void execute(const BrushToolParams& iParams, OctreeNodeDataParams& oParams) override
	{
		notifyObservers(iParams, oParams);
	}

	void undo() override
	{

	}

	static constexpr std::string_view getCommandName() noexcept { return "BrushToolCommand"; }
};