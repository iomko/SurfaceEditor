#include "ExportMeshesCommand.h"
#include "CommandRegistry.h"

void ExportMeshesCommand::execute(const ImportExportMeshesParams &params)
{
    notifyObservers(params);
}
static AutoRegister<ExportMeshesCommand> regExportMeshes("ExportMeshes");