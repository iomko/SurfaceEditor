#include "FaceDeselectionTool.h"
#include "../Tools/ToolRegistry.h"

FaceDeselectionTool::FaceDeselectionTool(CommandConcept* command)
    : Tool(command) {}
static AutoRegisterTool<FaceDeselectionTool> regFaceDeselectionTool;