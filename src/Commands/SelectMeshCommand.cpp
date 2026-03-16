#include "SelectMeshCommand.h"
#include "CommandRegistry.h"

void SelectMeshCommand::execute(const SelectionToolParams& iParams)
{
    notifyObservers(iParams);
}

void SelectMeshCommand::undo()
{
}

static AutoRegister<SelectMeshCommand> regSelectMesh;