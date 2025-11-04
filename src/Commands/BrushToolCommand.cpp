#include "BrushToolCommand.h"
#include "CommandRegistry.h"

void BrushToolCommand::execute(const BrushToolParams &iParams, OctreeNodeDataParams &oParams)
{
    notifyObservers(iParams, oParams);
}

void BrushToolCommand::undo()
{
}

static AutoRegister<BrushToolCommand> regBrushTool("BrushTool");
