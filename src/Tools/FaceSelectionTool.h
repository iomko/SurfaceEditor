#pragma once
#include "../Patterns/Command.h"
#include "../Tools/SelectionHandler.h"

class FaceSelectionTool : public Tool<CommandConcept, SelectionHandler<CommandConcept>>
{
public:
	FaceSelectionTool(CommandConcept* command)
		: Tool(command)
	{
	}
};
