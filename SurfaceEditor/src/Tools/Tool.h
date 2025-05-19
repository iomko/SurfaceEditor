#pragma once
#include "InteractionHandler.h"

class ITool
{
public:
	ITool(ICommand* command, IInteractionHandler* interactionHandler) :
		m_command(command), m_interactionHandler(interactionHandler){}

	virtual ~ITool() = default;

	virtual ICommand* getCommand()
	{
		return m_command;
	}
	virtual IInteractionHandler* getInteractionHandler()
	{
		return m_interactionHandler;
	}

private:
	ICommand* m_command = nullptr;
	IInteractionHandler* m_interactionHandler = nullptr;
};

template<typename CommandT, typename InteractionHandlerT>
class Tool : public ITool
{
public:
	Tool(CommandT* command, InteractionHandlerT* interactionHandler)
		: ITool(command, interactionHandler){}
};