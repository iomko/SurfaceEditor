#include "../../src/Commands/CommandRegistry.h"
#include "DeselectFaceCommand.h"

void DeselectFaceCommand::execute()
{
    notifyObservers();
}

void DeselectFaceCommand::undo()
{
}
static AutoRegister<DeselectFaceCommand> regDeselectFace("DESELECT_FACE_COMMAND");
