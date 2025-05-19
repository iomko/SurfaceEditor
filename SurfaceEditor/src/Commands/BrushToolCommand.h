#pragma once
#include "../Patterns/Observer.h"
#include "../Patterns/Command.h"
#include "CmdProperties/CmdProperties.h"


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