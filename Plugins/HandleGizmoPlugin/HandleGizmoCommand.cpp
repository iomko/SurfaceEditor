#include "HandleGizmoCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void HandleGizmoCommand::execute(const GizmoParams &iParams)
{
    notifyObservers(iParams);
}
void HandleGizmoCommand::undo()
{
}
static AutoRegister<HandleGizmoCommand> regHandleGizmoCommand("HANDLE_GIZMO_COMMAND");