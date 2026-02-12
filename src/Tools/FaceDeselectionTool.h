#pragma once
#include "../Tools/DeselectionHandler.h"
#include "../Patterns/Command.h"
#include "Tool.h"

class FaceDeselectionTool : public Tool<FACE_DESELECTION_TOOL, CommandConcept, DeselectionHandler<CommandConcept>>
{
public:
	FaceDeselectionTool(CommandConcept* command);
};
