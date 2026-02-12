#include "FaceSelectionTool.h"
#include "../Tools/ToolRegistry.h"

FaceSelectionTool::FaceSelectionTool(CommandConcept* command)
    : Tool(command) {}
static AutoRegisterTool<FaceSelectionTool> regFaceSelectionTool;