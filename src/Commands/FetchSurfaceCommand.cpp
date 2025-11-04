#include "FetchSurfaceCommand.h"
#include "CommandRegistry.h"

void FetchSurfaceCommand::execute(const OpenTopoParams &params)
{
    notifyObservers(params);
}

void FetchSurfaceCommand::undo()
{
}
static AutoRegister<FetchSurfaceCommand> regFetchSurface("FetchSurface");