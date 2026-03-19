#include "FaceSelectionTool.h"
#include "../../src/Tools/ToolRegistry.h"

FaceSelectionTool::FaceSelectionTool(CommandConcept* command)
    : Tool(command) {}
static AutoRegisterTool<FaceSelectionTool> regFaceSelectionTool("FACE_SELECTION_TOOL");