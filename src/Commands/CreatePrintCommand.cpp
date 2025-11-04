#include "CommandRegistry.h"
#include "CreatePrintCommand.h"

void CreatePrintCommand::execute(const PrintMeshSettingsParams &params)
{
    notifyObservers(params);
}

void CreatePrintCommand::undo()
{
}

static AutoRegister<CreatePrintCommand> regCreatePrint("CreatePrint");
