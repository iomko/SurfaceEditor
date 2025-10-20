#pragma once
#include "../Commands/BoxSelectionCommand.h"
#include "../Tools/BoxSelectionHandler.h"
#include "Tool.h"

class BoxSelectionTool : public Tool<BoxSelectionCommand, BoxSelectionHandler<BoxSelectionCommand>>
{
public:
	BoxSelectionTool(BoxSelectionCommand* command)
		: Tool(command)
	{
	}
};