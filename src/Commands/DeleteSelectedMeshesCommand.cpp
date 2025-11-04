#include "DeleteSelectedMeshesCommand.h"
#include "CommandRegistry.h"

void DeleteSelectedMeshesCommand::execute()
{
    notifyObservers();
}

void DeleteSelectedMeshesCommand::undo()
{
}
static AutoRegister<DeleteSelectedMeshesCommand> regDeleteSelectedMeshes("DeleteSelectedMeshes");
