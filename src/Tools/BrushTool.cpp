#include "BrushTool.h"
#include "../Tools/ToolRegistry.h"

BrushTool::BrushTool(CommandConcept* command)
    : Tool(command) {}

static AutoRegisterTool<BrushTool> regBrushTool;