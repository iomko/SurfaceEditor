#pragma once
#include "../../src/Tools/DeselectionHandler.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Tools/Tool.h"
#include "../../src/Tools/ToolIDs.h"

class FaceDeselectionTool : public Tool<FACE_DESELECTION_TOOL, CommandConcept, DeselectionHandler<CommandConcept>>
{
public:
	FaceDeselectionTool(CommandConcept* command);
};
