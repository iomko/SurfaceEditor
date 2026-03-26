#include "MeshDeselectionTool.h"
#include "../../src/Tools/ToolRegistry.h"

MeshDeselectionTool::MeshDeselectionTool(CommandConcept* command)
    : Tool(command) {}
static AutoRegisterTool<MeshDeselectionTool> regMeshDeselectionTool("MESH_DESELECTION_TOOL");