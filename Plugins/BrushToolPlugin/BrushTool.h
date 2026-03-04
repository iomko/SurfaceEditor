#pragma once
#include "../../src/Tools/Tool.h"
#include "../../src/Tools/ToolIDs.h"
#include "../../src/Patterns/Command.h"
#include "BrushInteractionHandler.h"

class BrushTool : public Tool<BRUSH_TOOL, CommandConcept, BrushInteractionHandler>
{
public:
	BrushTool(CommandConcept* command);

};
