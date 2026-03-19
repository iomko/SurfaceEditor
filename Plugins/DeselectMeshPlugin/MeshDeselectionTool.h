#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Tools/DeselectionHandler.h"
#include "../../src/Tools/Tool.h"

class MeshDeselectionTool : public Tool<CommandConcept, DeselectionHandler<CommandConcept>>
{
public:
	MeshDeselectionTool(CommandConcept* command);
};
