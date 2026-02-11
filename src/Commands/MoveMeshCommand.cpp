#include "MoveMeshCommand.h"
#include "CommandRegistry.h"

static AutoRegister<MoveMeshCommand> regMoveMeshCommand;

void MoveMeshCommand::execute(const MoveMeshParams &params)
{
    notifyObservers(params);
}

void MoveMeshCommand::undo() override
{
}