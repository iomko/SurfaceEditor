#include "BrushTool.h"
#include "../../src/Tools/ToolRegistry.h"

BrushTool::BrushTool(CommandConcept* command)
    : Tool(command) {}

static AutoRegisterTool<BrushTool> regBrushTool;