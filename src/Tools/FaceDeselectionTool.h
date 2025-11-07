#pragma once
#include "../Tools/DeselectionHandler.h"
#include "Tool.h"

class FaceDeselectionTool : public Tool<CommandConcept, DeselectionHandler<CommandConcept>>
{
public:
	FaceDeselectionTool(CommandConcept* command)
		: Tool(command)
	{
	}
};
