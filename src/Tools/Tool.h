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

template<int id, typename CommandConcept, typename InteractionHandlerT>
class Tool : public ITool
{
public:
	static constexpr int ID = id;
	Tool(CommandConcept* command)
		: ITool(new InteractionHandlerT(command)) {}
};