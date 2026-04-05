#include "MoveVertexCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void MoveVertexCommand::execute(const VertexParams& params)
{
    notifyObservers(params);
}

void MoveVertexCommand::undo()
{
}
static AutoRegister<MoveVertexCommand> regMoveVertex("MOVE_VERTEX_COMMAND");