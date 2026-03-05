#pragma once
#include "../Patterns/Command.h"
#include "DeselectionHandler.h"
#include "Tool.h"

class MeshDeselectionTool : public Tool<MESH_DESELECTION_TOOL, CommandConcept, DeselectionHandler<CommandConcept>>
{
public:
	MeshDeselectionTool(CommandConcept* command);
};
