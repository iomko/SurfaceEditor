#pragma once
#include "../Patterns/Command.h"
#include "SelectionHandler.h"
#include "Tool.h"

class MeshSelectionTool : public Tool<MESH_SELECTION_TOOL, CommandConcept, SelectionHandler<CommandConcept>>
{
public:
	MeshSelectionTool(CommandConcept* command);
};
