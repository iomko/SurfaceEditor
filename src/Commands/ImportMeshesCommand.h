#pragma once
#include <string_view>
#include "Patterns/Command.h"
#include "Patterns/Observer.h"


class ImportMeshesCommand : public Observable, public Command<IMPORT_MESHES_COMMAND, ImportExportMeshesParams>
{
public:
	virtual void execute(const ImportExportMeshesParams& params) override;
};
