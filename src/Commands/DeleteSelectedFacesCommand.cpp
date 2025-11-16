#include "DeleteSelectedFacesCommand.h"
#include "CommandRegistry.h"

void DeleteSelectedFacesCommand::execute()
{
    notifyObservers();
}

void DeleteSelectedFacesCommand::undo()
{
}
static AutoRegister<DeleteSelectedFacesCommand> regDeleteSelectedFaces;
