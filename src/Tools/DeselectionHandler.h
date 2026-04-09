#pragma once
#include "InteractionHandler.h"

template<typename DeselectCommandT>
class DeselectionHandler : public InteractionHandler<DeselectCommandT>
{
public:
	DeselectionHandler(DeselectCommandT* command)
		: InteractionHandler<DeselectCommandT>(command) {}

	void onBegin() override
	{
	}

	void onUpdate() override
	{
		this->getCommand()->execute();
	}
	
	void onEnd() override {}
};