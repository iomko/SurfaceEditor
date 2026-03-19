#include "SolidifyMeshesCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void SolidifyMeshesCommand::execute()
{
    notifyObservers();
}

void SolidifyMeshesCommand::undo()
{
}
static AutoRegister<SolidifyMeshesCommand> regSolidifyMeshes("SOLIDIFY_MESHES_COMMAND");