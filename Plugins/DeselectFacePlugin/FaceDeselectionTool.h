#pragma once
#include "../../src/Tools/DeselectionHandler.h"
#include "../../src/Patterns/Command.h"
#include "../../src/Tools/Tool.h"

class FaceDeselectionTool : public Tool<CommandConcept, DeselectionHandler<CommandConcept>>
{
public:
	FaceDeselectionTool(CommandConcept* command);
};
