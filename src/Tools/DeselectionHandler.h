#pragma once
#include "InteractionHandler.h"

template<typename DeselectCommandT>
class DeselectionHandler : public InteractionHandler<DeselectCommandT, SelectionToolParams>
{
public:
	DeselectionHandler(DeselectCommandT* command)
		: InteractionHandler<DeselectCommandT, SelectionToolParams>(command) {}

	void onBegin(const SelectionToolParams& iParams) override
	{
		// this->getCommand()->execute(iParams);
	}

	void onUpdate(const SelectionToolParams& iParams) override
	{
		this->getCommand()->execute(iParams);
	}
	
	void onEnd(const SelectionToolParams& iParams) override {}
};