#pragma once
#include "Tool.h"
#include "../Patterns/Command.h"
#include "BrushInteractionHandler.h"

class BrushTool : public Tool<BRUSH_TOOL, CommandConcept, BrushInteractionHandler>
{
public:
	BrushTool(CommandConcept* command);

};
