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
		this->getCommand()->execute();
	}
	void onUpdate() override {}
	void onEnd() override {}
};