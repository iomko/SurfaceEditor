#include "ImportMeshesCommand.h"
#include "../../src/Commands/CommandRegistry.h"

void ImportMeshesCommand::execute(const ImportExportMeshesParams &params)
{
    notifyObservers(params);
}
static AutoRegister<ImportMeshesCommand> regImportMeshes("IMPORT_MESHES_COMMAND");