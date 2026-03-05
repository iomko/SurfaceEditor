#pragma once
#include "InteractionHandler.h"
#include "../Commands/CommandRegistry.h"

template<typename CommandConcept>
class SelectionHandler : public InteractionHandler<CommandConcept>
{
public:
	SelectionHandler(CommandConcept* command)
		: InteractionHandler<CommandConcept>(command) {}

	void onBegin() override
	{
		this->getCommand()->execute();
	}
	void onUpdate() override {}
	void onEnd() override {}
};