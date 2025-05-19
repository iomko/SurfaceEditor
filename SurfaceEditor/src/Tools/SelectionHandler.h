#pragma once
#include "InteractionHandler.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/SelectFaceCommand.h"

template<typename SelectCommandT>
class SelectionHandler : public InteractionHandler<>
{
public:
	void onBegin() override
	{
		SelectCommandT* selectCommand = CommandRegistry::getCommand<SelectCommandT>();
		selectCommand->execute();
	}
	void onUpdate() override {}
	void onEnd() override {}
};