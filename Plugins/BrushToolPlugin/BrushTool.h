#pragma once
#include "../../src/Tools/Tool.h"
#include "../../src/Patterns/Command.h"
#include "BrushInteractionHandler.h"

class BrushTool : public Tool<CommandConcept, BrushInteractionHandler>
{
public:
	BrushTool(CommandConcept* command);

};
