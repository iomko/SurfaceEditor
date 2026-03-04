#include "DeleteFaceCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void DeleteFaceCommand::execute(const SingleFaceParams &params)
{
    notifyObservers(params);
}

void DeleteFaceCommand::undo()
{
}
static AutoRegister<DeleteFaceCommand> regDeleteFace;
