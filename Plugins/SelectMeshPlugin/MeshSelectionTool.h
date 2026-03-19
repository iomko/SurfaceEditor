#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Tools/Tool.h"
#include "../../src/Tools/SelectionHandler.h"

class MeshSelectionTool : public Tool<CommandConcept, SelectionHandler<CommandConcept>>
{
public:
	MeshSelectionTool(CommandConcept* command);
};
