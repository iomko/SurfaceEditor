#pragma once
#include "../Commands/SelectFaceCommand.h"
#include "../Tools/SelectionHandler.h"

class FaceSelectionTool : public Tool<SelectFaceCommand, SelectionHandler<SelectFaceCommand>>
{
public:
	FaceSelectionTool(SelectFaceCommand* command)
		: Tool(command)
	{
	}
};
