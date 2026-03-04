#include "DeleteSelectedMeshesCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void DeleteSelectedMeshesCommand::execute()
{
    notifyObservers();
}

void DeleteSelectedMeshesCommand::undo()
{
}
static AutoRegister<DeleteSelectedMeshesCommand> regDeleteSelectedMeshes;
