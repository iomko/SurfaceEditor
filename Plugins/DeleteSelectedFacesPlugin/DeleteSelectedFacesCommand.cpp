#include "DeleteSelectedFacesCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void DeleteSelectedFacesCommand::execute()
{
    notifyObservers();
}

void DeleteSelectedFacesCommand::undo()
{
}
static AutoRegister<DeleteSelectedFacesCommand> regDeleteSelectedFaces;
