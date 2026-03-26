#include "DeleteSelectedFacesCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void DeleteSelectedFacesCommand::execute()
{
    notifyObservers();
}

void DeleteSelectedFacesCommand::undo()
{
}
static AutoRegister<DeleteSelectedFacesCommand> regDeleteSelectedFaces("DELETE_SELECTED_FACES_COMMAND");
