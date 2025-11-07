#pragma once
#include "../Patterns/Command.h"
#include "../Tools/SelectionHandler.h"

class MeshSelectionTool : public Tool<CommandConcept, SelectionHandler<CommandConcept>>
{
public:
	MeshSelectionTool(CommandConcept* command)
		: Tool(command)
	{
	}
};
