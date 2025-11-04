#pragma once
#include "../Patterns/Command.h"
#include "../Patterns/Observer.h"

class ExportMeshesCommand : public Command<ImportExportMeshesParams>, public Observable
{
public:
	virtual void execute(const ImportExportMeshesParams& params) override;
};
