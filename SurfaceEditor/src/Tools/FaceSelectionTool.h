#pragma once
#include "../Commands/SelectFaceCommand.h"
#include "../Tools/SelectionHandler.h"

class FaceSelectionTool : public Tool<SelectFaceCommand, SelectionHandler<SelectFaceCommand>>
{
public:
	FaceSelectionTool(SelectFaceCommand* command, SelectionHandler<SelectFaceCommand>* interactionHandler)
		: Tool(command, interactionHandler)
	{
	}
	static std::string getToolName() { return "FaceSelectionTool"; }
};