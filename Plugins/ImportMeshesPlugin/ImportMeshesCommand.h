#pragma once
#include <string_view>
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"


class ImportMeshesCommand : public Observable, public Command<IMPORT_MESHES_COMMAND, ImportExportMeshesParams>
{
public:
	virtual void execute(const ImportExportMeshesParams& params) override;
};
