#include "MoveSelectedMeshesCommand.h"
#include "../../src/Commands/CommandRegistry.h"

static AutoRegister<MoveSelectedMeshesCommand> regMoveSelectedMeshesCommand("MOVE_SELECTED_MESHES_COMMAND");
void MoveSelectedMeshesCommand::execute(const MoveSelectedMeshesParams &iParams)
{
    notifyObservers(iParams);
}
void MoveSelectedMeshesCommand::undo()
{}