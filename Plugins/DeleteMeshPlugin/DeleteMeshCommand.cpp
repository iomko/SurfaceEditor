#include "DeleteMeshCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void DeleteMeshCommand::execute(const MeshParams &params)
{
    notifyObservers(params);
}

void DeleteMeshCommand::undo()
{}

static AutoRegister<DeleteMeshCommand> regDeleteMesh("DELETE_MESH_COMMAND");
