#include "MeshDeselectionTool.h"
#include "../Tools/ToolRegistry.h"

MeshDeselectionTool::MeshDeselectionTool(CommandConcept* command)
    : Tool(command) {}
static AutoRegisterTool<MeshDeselectionTool> regMeshDeselectionTool;