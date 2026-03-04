#include "FaceDeselectionTool.h"
#include "../../src/Tools/ToolRegistry.h"

FaceDeselectionTool::FaceDeselectionTool(CommandConcept* command)
    : Tool(command) {}
static AutoRegisterTool<FaceDeselectionTool> regFaceDeselectionTool;