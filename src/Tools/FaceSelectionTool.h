#pragma once
#include "../Patterns/Command.h"
#include "../Tools/SelectionHandler.h"
#include "Tool.h"

class FaceSelectionTool : public Tool<FACE_SELECTION_TOOL, CommandConcept, SelectionHandler<CommandConcept>>
{
public:
	FaceSelectionTool(CommandConcept* command);
};
