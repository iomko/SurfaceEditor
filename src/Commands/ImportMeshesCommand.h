#pragma once
#include <string_view>
#include "Patterns/Command.h"
#include "Patterns/Observer.h"


class ImportMeshesCommand : public Observable, public Command<ImportExportMeshesParams>
{
public:
	virtual void execute(const ImportExportMeshesParams& params) override;
};
