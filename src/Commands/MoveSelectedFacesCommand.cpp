#include "MoveSelectedFacesCommand.h"
#include "CommandRegistry.h"

void MoveSelectedFacesCommand::execute(const MoveSelectedFacesParams& iParams)
{
    printf("COMMAND\n");
    notifyObservers(iParams);
}

void MoveSelectedFacesCommand::undo()
{
}
static AutoRegister<MoveSelectedFacesCommand> regMoveSelectedFaces;