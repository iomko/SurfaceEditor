#include "DeselectMeshCommand.h"
#include "CommandRegistry.h"

void DeselectMeshCommand::execute(const SelectionToolParams& iParams)
{
    notifyObservers(iParams);
}

void DeselectMeshCommand::undo()
{
}
static AutoRegister<DeselectMeshCommand> regDeselectMesh;