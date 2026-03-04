#pragma once
#include "../../src/Patterns/Command.h"
#include "../../src/Patterns/Observer.h"
#include "../../src/Commands/CommandIDs.h"

class ExportMeshesCommand : public Command<EXPORT_MESHES_COMMAND,ImportExportMeshesParams>, public Observable
{
public:
	virtual void execute(const ImportExportMeshesParams& params) override;
};
