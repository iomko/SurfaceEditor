#pragma once
#include "../Commands/SelectMeshCommand.h"
#include "../Tools/SelectionHandler.h"

class MeshSelectionTool : public Tool<SelectMeshCommand, SelectionHandler<SelectMeshCommand>>
{
public:
	MeshSelectionTool(SelectMeshCommand* command)
		: Tool(command)
	{
	}
};
