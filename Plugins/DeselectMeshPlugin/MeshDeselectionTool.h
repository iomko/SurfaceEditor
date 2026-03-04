#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Tools/DeselectionHandler.h"
#include "../../src/Tools/Tool.h"
#include "../../src/Tools/ToolIDs.h"

class MeshDeselectionTool : public Tool<MESH_DESELECTION_TOOL, CommandConcept, DeselectionHandler<CommandConcept>>
{
public:
	MeshDeselectionTool(CommandConcept* command);
};
