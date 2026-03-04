#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Tools/SelectionHandler.h"
#include "../../src/Tools/Tool.h"
#include "../../src/Tools/ToolIDs.h"

class FaceSelectionTool : public Tool<FACE_SELECTION_TOOL, CommandConcept, SelectionHandler<CommandConcept>>
{
public:
	FaceSelectionTool(CommandConcept* command);
};
