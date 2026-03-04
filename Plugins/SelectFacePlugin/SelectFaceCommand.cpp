#include "SelectFaceCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void SelectFaceCommand::execute()
{
    notifyObservers();
}

void SelectFaceCommand::undo()
{
}

static AutoRegister<SelectFaceCommand> regSelectFace;