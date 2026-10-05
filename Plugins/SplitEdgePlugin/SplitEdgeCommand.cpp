#include "SplitEdgeCommand.h"
#include "../../src/Commands/CommandRegistry.h"

static AutoRegister<SplitEdgeCommand> regSplitEdgeCommand("SPLIT_EDGE_COMMAND");

void SplitEdgeCommand::execute(const SplitEdgeParams& params)
{
    notifyObservers(params);
}

void SplitEdgeCommand::undo()
{

}