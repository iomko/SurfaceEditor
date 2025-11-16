#include "MoveVertexCommand.h"
#include "CommandRegistry.h"

void MoveVertexCommand::execute(const VertexParams& params)
{
    notifyObservers(params);
}

void MoveVertexCommand::undo()
{
}
static AutoRegister<MoveVertexCommand> regMoveVertex;