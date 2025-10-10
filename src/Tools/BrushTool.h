#pragma once
#include "../Commands/BrushToolCommand.h"
#include "BrushInteractionHandler.h"

class BrushTool : public Tool<BrushToolCommand, BrushInteractionHandler>
{
public:
	BrushTool(BrushToolCommand* command)
		: Tool(command) {}

};
