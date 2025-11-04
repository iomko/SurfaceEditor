#include "ImportMeshesCommand.h"
#include "CommandRegistry.h"

void ImportMeshesCommand::execute(const ImportExportMeshesParams &params)
{
    notifyObservers(params);
}
static AutoRegister<ImportMeshesCommand> regImportMeshes("ImportMeshes");