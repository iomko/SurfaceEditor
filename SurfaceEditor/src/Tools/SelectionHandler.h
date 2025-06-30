#pragma once
#include "InteractionHandler.h"
#include "../Commands/CommandRegistry.h"
#include "../Commands/SelectFaceCommand.h"

template<typename SelectCommandT>
class SelectionHandler : public InteractionHandler<SelectCommandT>
{
public:
	SelectionHandler(SelectCommandT* command)
		: InteractionHandler<SelectCommandT>(command) {}

	void onBegin() override
	{
		this->getCommand()->execute();
	}
	void onUpdate() override {}
	void onEnd() override {}
};