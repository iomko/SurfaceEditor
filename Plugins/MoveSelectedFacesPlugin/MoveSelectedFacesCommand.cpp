#include "MoveSelectedFacesCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void MoveSelectedFacesCommand::execute(const MoveSelectedFacesParams& iParams)
{
    notifyObservers(iParams);
}

void MoveSelectedFacesCommand::undo()
{
}
static AutoRegister<MoveSelectedFacesCommand> regMoveSelectedFaces("MOVE_SELECTED_FACES_COMMAND");