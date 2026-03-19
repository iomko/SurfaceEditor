#include "SelectMeshCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void SelectMeshCommand::execute()
{
    notifyObservers();
}

void SelectMeshCommand::undo()
{
}
static AutoRegister<SelectMeshCommand> regSelectMesh("SELECT_MESH_COMMAND");