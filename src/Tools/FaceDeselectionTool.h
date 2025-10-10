#pragma once
#include "../Commands/DeselectFaceCommand.h"
#include "../Tools/DeselectionHandler.h"
#include "Tool.h"

class FaceDeselectionTool : public Tool<DeselectFaceCommand, DeselectionHandler<DeselectFaceCommand>>
{
public:
	FaceDeselectionTool(DeselectFaceCommand* command)
		: Tool(command)
	{
	}
};
