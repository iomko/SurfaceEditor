#pragma once
#include "../Patterns/Command.h"
#include "BrushInteractionHandler.h"

class BrushTool : public Tool<CommandConcept, BrushInteractionHandler>
{
public:
	BrushTool(CommandConcept* command)
		: Tool(command) {}

};
