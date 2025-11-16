#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class ExportMeshesCommand : public Command<EXPORT_MESHES_COMMAND,ImportExportMeshesParams>, public Observable
{
public:
	virtual void execute(const ImportExportMeshesParams& params) override;
};
