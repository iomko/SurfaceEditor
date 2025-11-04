#include "SolidifyMeshesCommand.h"
#include "CommandRegistry.h"

void SolidifyMeshesCommand::execute()
{
    notifyObservers();
}

void SolidifyMeshesCommand::undo()
{
}
static AutoRegister<SolidifyMeshesCommand> regSolidifyMeshes("SolidifyMeshes");