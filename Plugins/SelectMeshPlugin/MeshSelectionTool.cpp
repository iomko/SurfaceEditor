#include "MeshSelectionTool.h"
#include "../../src/Tools/ToolRegistry.h"

MeshSelectionTool::MeshSelectionTool(CommandConcept* command)
    : Tool(command) {}
static AutoRegisterTool<MeshSelectionTool> regMeshSelectionTool("MESH_SELECTION_TOOL");