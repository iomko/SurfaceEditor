#include "ExportMeshesCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void ExportMeshesCommand::execute(const ImportExportMeshesParams &params)
{
    notifyObservers(params);
}
static AutoRegister<ExportMeshesCommand> regExportMeshes("EXPORT_MESHES_COMMAND");