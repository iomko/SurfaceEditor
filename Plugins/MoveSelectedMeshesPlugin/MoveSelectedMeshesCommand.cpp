#include "MoveSelectedMeshesCommand.h"
#include "../../src/Commands/CommandRegistry.h"

static AutoRegister<MoveSelectedMeshesCommand> regMoveSelectedMeshesCommand;
void MoveSelectedMeshesCommand::execute(const MoveSelectedMeshesParams &iParams)
{
    notifyObservers(iParams);
}
void MoveSelectedMeshesCommand::undo()
{}