#include "MoveFaceCommand.h"
#include "CommandRegistry.h"

void MoveFaceCommand::execute(const SingleFaceParams &params)
{
    notifyObservers(params);
}

void MoveFaceCommand::undo()
{
}
static AutoRegister<MoveFaceCommand> regMoveFace("MoveFace");