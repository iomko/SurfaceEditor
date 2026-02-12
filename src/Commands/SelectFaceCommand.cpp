#include "SelectFaceCommand.h"
#include "CommandRegistry.h"

void SelectFaceCommand::execute()
{
    notifyObservers();
}

void SelectFaceCommand::undo()
{
}

static AutoRegister<SelectFaceCommand> regSelectFace;