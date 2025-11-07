#pragma once
#include "../Patterns/Command.h"
#include "../Tools/DeselectionHandler.h"

class MeshDeselectionTool : public Tool<CommandConcept, DeselectionHandler<CommandConcept>>
{
public:
	MeshDeselectionTool(CommandConcept* command)
		: Tool(command)
	{
	}
};
