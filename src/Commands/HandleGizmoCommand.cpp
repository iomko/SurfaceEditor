#include "HandleGizmoCommand.h"
#include "CommandRegistry.h"

void HandleGizmoCommand::execute(const GizmoParams &iParams)
{
    notifyObservers(iParams);
}
void HandleGizmoCommand::undo()
{
}
static AutoRegister<HandleGizmoCommand> regHandleGizmoCommand;