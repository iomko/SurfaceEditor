#include "SplitEdgeCommand.h"
#include "CommandRegistry.h"
void SplitEdgeCommand::execute(const SplitEdgeParams& params)
{
    notifyObservers(params);
}

void SplitEdgeCommand::undo()
{

}
static AutoRegister<SplitEdgeCommand> regSplitEdge;