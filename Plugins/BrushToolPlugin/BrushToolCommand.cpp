#include "BrushToolCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void BrushToolCommand::execute(const BrushToolParams &iParams, OctreeNodeDataParams &oParams)
{
    notifyObservers(iParams, oParams);
}

void BrushToolCommand::undo()
{
}

static AutoRegister<BrushToolCommand> regBrushTool("BRUSH_TOOL_COMMAND");
