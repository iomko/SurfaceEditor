#pragma once
#include "../Commands/DeselectMeshCommand.h"
#include "../Tools/DeselectionHandler.h"

class MeshDeselectionTool : public Tool<DeselectMeshCommand, DeselectionHandler<DeselectMeshCommand>>
{
public:
	MeshDeselectionTool(DeselectMeshCommand* command)
		: Tool(command)
	{
	}
};
