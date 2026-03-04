#include "DeselectMeshCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void DeselectMeshCommand::execute()
{
    notifyObservers();
}

void DeselectMeshCommand::undo()
{
}
static AutoRegister<DeselectMeshCommand> regDeselectMesh;