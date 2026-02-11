#include "HandleGizmoCommand.h"
#include "CommandRegistry.h"

void HandleGizmoCommand::execute(const GizmoParams &iParams)
{
    notifyObservers(iParams);
}
void HandleGizmoCommand::undo() override
{
}
static AutoRegister<HandleGizmoCommand> regHandleGizmoCommand;