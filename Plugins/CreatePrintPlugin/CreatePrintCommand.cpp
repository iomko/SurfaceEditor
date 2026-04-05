#include "../../src/Commands/CommandRegistry.h"
#include "CreatePrintCommand.h"

void CreatePrintCommand::execute(const PrintMeshSettingsParams &params)
{
    notifyObservers(params);
}

void CreatePrintCommand::undo()
{
}

static AutoRegister<CreatePrintCommand> regCreatePrint("CREATE_PRINT_COMMAND");
