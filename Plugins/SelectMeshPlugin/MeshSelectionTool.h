#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Tools/Tool.h"
#include "../../src/Tools/ToolIDs.h"
#include "../../src/Tools/SelectionHandler.h"

class MeshSelectionTool : public Tool<MESH_SELECTION_TOOL, CommandConcept, SelectionHandler<CommandConcept>>
{
public:
	MeshSelectionTool(CommandConcept* command);
};
