#pragma once
#include "InteractionHandler.h"
#include "../Commands/CommandRegistry.h"

template<typename CommandConcept>
class SelectionHandler : public InteractionHandler<CommandConcept, SelectionToolParams>
{
public:
	SelectionHandler(CommandConcept* command)
		: InteractionHandler<CommandConcept, SelectionToolParams>(command) {}

	void onBegin(const SelectionToolParams& iParams) override
	{
		//
	}
	void onUpdate(const SelectionToolParams& iParams) override
	{
		this->getCommand()->execute(iParams);
	}

	void onEnd(const SelectionToolParams& iParams) override {}
};