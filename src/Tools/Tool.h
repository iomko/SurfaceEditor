#pragma once
#include "InteractionHandler.h"

class ITool
{
public:
	ITool(InteractionHandlerConcept* interactionHandler) :
	m_interactionHandler(interactionHandler){}

	virtual ~ITool() = default;

	virtual InteractionHandlerConcept* getInteractionHandler()
	{
		return m_interactionHandler;
	}

private:
	InteractionHandlerConcept* m_interactionHandler = nullptr;
};

template<typename CommandT, typename InteractionHandlerT>
class Tool : public ITool
{
public:
	Tool(CommandT* command)
		: ITool(new InteractionHandlerT(command)) {}
};