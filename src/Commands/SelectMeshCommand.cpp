#include "SelectMeshCommand.h"
#include "CommandRegistry.h"

void SelectMeshCommand::execute()
{
    notifyObservers();
}

void SelectMeshCommand::undo()
{
}
static AutoRegister<SelectMeshCommand> regSelectMesh;