#include "DeleteMeshCommand.h"
#include "CommandRegistry.h"

void DeleteMeshCommand::execute(const MeshParams &params)
{
    notifyObservers(params);
}

void DeleteMeshCommand::undo()
{}

static AutoRegister<DeleteMeshCommand> regDeleteMesh;
