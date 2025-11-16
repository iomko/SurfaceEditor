#include "DeselectMeshCommand.h"
#include "CommandRegistry.h"

void DeselectMeshCommand::execute()
{
    notifyObservers();
}

void DeselectMeshCommand::undo()
{
}
static AutoRegister<DeselectMeshCommand> regDeselectMesh;