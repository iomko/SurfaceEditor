#include "MeshSelectionTool.h"
#include "../Tools/ToolRegistry.h"

MeshSelectionTool::MeshSelectionTool(CommandConcept* command)
    : Tool(command) {}
static AutoRegisterTool<MeshSelectionTool> regMeshSelectionTool;