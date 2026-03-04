#include "MoveMeshCommand.h"
#include "../../src/Commands/CommandRegistry.h"

static AutoRegister<MoveMeshCommand> regMoveMeshCommand;

void MoveMeshCommand::execute(const MoveMeshParams &params)
{
    notifyObservers(params);
}

void MoveMeshCommand::undo()
{
}